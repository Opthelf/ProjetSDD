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

int octalVersDecimal(int octal){
    int decimal = 0;
    int i = 0;
    while(i*64 <= octal){
        i++;
    }
    octal = octal%64;
    decimal += 100*(i-1);
    i = 0;
    while(i*8 <= octal){
        i++;
    }
    octal = octal%8;
    decimal += 10*(i-1);
    i = 0;
    while(i <= octal){
        i++;
    }
    decimal += i-1;
    return decimal;
}

int getChmod(const char * path){
    struct stat ret;
    if(stat(path,&ret)==-1){
        return -1;
    }
    int octal = (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);
    return octalVersDecimal(octal);
}

void setMode(int mode, char * path){
    char buff[100];
    sprintf(buff,"chmod %d %s",mode,path);
    system(buff);
}

char * hashToFile(char *hash){
    char * ch2 = strdup(hash);
    ch2[2]='\0';
    struct stat st={0};
    if(stat(ch2,&st) == -1){
        mkdir(ch2,0700);
    }
    free(ch2);
    return hashToPath(hash);
}

char * blobWorkTree(WorkTree *wt){
    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);
    wttf(wt,fname);
    char * hash = sha256file(fname);
    char * ch = hashToFile(hash);
    strcat(ch,".t");
    cp(ch,fname);
    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);
    return hash;
}


char * concat_paths(char * path1,char * path2){
    char * result = malloc((strlen(path1)+strlen(path2)+1)*sizeof(char));
    if (result == NULL){
        printf("Erreur d'allocation mémoire\n");
        return NULL;
    }
    strcpy(result,path1);
    strcat(result,"/");
    strcat(result,path2);
    return result;
}

int isFile(const char *path){
    struct stat path_stat;
    stat(path,&path_stat);
    return S_ISREG(path_stat.st_mode);
}

char * saveWorkTree(WorkTree *wt,char * path){
    for(int i=0;i<wt->n;i++){
        char * absPath = concat_paths(path,wt->tab[i].name);
        if(isFile(absPath)==1){
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
                appendWorkTree(wt2,ptr->data,NULL,777);
            }
            wt->tab[i].hash = saveWorkTree(wt2,absPath);
            wt->tab[i].mode = getChmod(absPath);
        }
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
                printf("wt : %s\n",wtts(wt));
                WorkTree * newwt = ftwt(copyPath);
                restoreWorkTree(newwt,absPath);
                setMode(getChmod(copyPath),absPath);
            }
        }
    }
}