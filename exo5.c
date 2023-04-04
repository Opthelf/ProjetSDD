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
char * hashToFile(char *hash){
    char * ch2 = strdup(hash);
    ch2[2]='\0';
    struct stat st={0};
    if(stat(ch2,&st) == -1){
        mkdir(ch2,0700);
    }
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
    return hash;
    }


char * concat_paths(char * path1,char * path2){
    char * result= malloc((strlen(path1)+strlen(path2)+1)*sizeof(char));
    if (result ==NULL){
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

/*char * saveWorkTree(WorkTree *wt,char * path){
    for(int i=0;i<wt->n;i++){
        char * absPath= concat_paths(path,wt->tab[i].name);
        if(isFile)
    }
}*/