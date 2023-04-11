#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"

char * blobWorkTree(WorkTree *wt){
    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);
    wttf(wt,fname);
    char * hash = sha256file(fname);
    char * ch = hashToFile(hash);
    ch = realloc(ch,(strlen(ch)+3)*sizeof(char));
    strcat(ch,".t");
    cp(ch,fname);
    setMode(777,ch);

    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);
    free(ch);
    return hash;
}


char * concat_paths(char * path1,char * path2){
    int len = strlen(path1)+ strlen(path2)+2;

    char * result = malloc(len*sizeof(char));
    if (result == NULL){
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    } 
    strcpy(result,"");
    strcat(result,path1);
    strcat(result,"/");
    strcat(result,path2);
    strcat(result,"\0");
    return result;
}

char * saveWorkTree(WorkTree *wt,char * path){
    char * absPath;
    for(int i=0;i<wt->n-1;i++){
        absPath = concat_paths(path,wt->tab[i].name);
        if(isFile(absPath) == 1){
            blobFile(absPath);
            wt->tab[i].hash = sha256file(absPath);
            wt->tab[i].mode = getChmod(absPath);
        }else{
            WorkTree *wt2 = initWorkTree();
            List * L = listdir(absPath);
            for(Cell * ptr = *L;ptr !=NULL ; ptr = ptr->next){
                if(ptr->data[0]=='.'){
                    continue;
                }
                char buff[1000];
                strcpy(buff,ptr->data);
                appendWorkTree(wt2,buff,NULL,777);
            }
            wt->tab[i].hash = saveWorkTree(wt2,absPath);
            wt->tab[i].mode = getChmod(absPath);
            freeWorkTree(wt2);
        }
    free(absPath);
    }
    
    return blobWorkTree(wt);
}

int isWorkTree(char* hash){
    if (file_exists(strcat(hashToPath(hash),".t"))){
        return 1; 
    }
    if (file_exists(hashToPath(hash))){ 
        return 0;
    }
    return -1; 
}

void restoreWorkTree(WorkTree * wt, char * path){
    for(int i = 0 ; i < wt->n ; i ++){
        char * absPath = concat_paths(path,wt->tab[i].name);
        char * copyPath = hashToPath(wt->tab[i].hash);
        char * hash = wt->tab[i].hash;
        if (isWorkTree(wt->tab[i].hash) == 0){
            cp(absPath,copyPath);
            setMode(getChmod(copyPath),absPath);
        }
        else{
            if (isWorkTree(wt->tab[i].hash) == 1 ){
                strcat(copyPath,".t");
                WorkTree * newwt = ftwt(copyPath);
                restoreWorkTree(newwt,absPath);
                setMode(getChmod(copyPath),absPath);
            }
        }
    }
}