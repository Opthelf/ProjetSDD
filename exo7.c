#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"


void initRefs(){
    if(!file_exists(".refs")){
        system("mkdir .refs");
        system("touch .refs/master");
        system("touch .refs/HEAD");    
    }
}

void createUpdateRef(char* ref_name, char* hash){
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,"echo %s > .refs/%s",hash,ref_name);
    system(buff);
}

void deleteRef(char* ref_name){
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(deleteRef)\n",buff);
        return;
    }
    sprintf(buff,"rm .refs/%s",ref_name);
    system(buff);
}

char* getRef(char* ref_name){
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(getRef)\n",buff);
        return NULL;
    }
    FILE * f = fopen(buff,"r");
    if (f == NULL){
        printf("Erreur %d d'ouverture lors du fichier(getRef)\n",errno);
        return NULL;
    }
    char * recup = malloc(sizeof(char)*100);
    strcpy(recup,"");
    if (fgets(recup,256,f) == NULL){
        printf("Le fichier %s est vide !(getRef)\n",buff);
    }
    return recup;
}