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
#include "exo8.h"

void initBranch(){
    system("echo master > .current_branch");
}

int branchExists(char* branch){
    if (file_exists(".refs") == 0){
        printf("Dossier .refs non initialisé, aucunes branches existantes -> branchExists\n");
        return 0;
    }
    char buff[100];
    strcpy(buff,"");
    strcat(buff,".refs/");
    strcat(buff,branch);
    if (file_exists(buff) == 0){
        printf("La branche %s n'existe pas -> branchExists\n",branch);
        return 0;
    }
    return 1;
}

void createBranch(char* branch){
    char * chaine = getRef("HEAD");
    printf("%s\n",chaine);
    createUpdateRef(branch,chaine);
    free(chaine);
}

char* getCurrentBranch(){
    if (file_exists(".current_branch") == 0){
        printf("Fichier .current_branch non-initialisé -> getCurrentBranch\n");
        return NULL;
    }
    FILE * f = fopen(".current_branch","r");
    if (f == NULL){
        printf("Erreur lors de l'ouverture du fichier -> getCurrentBranch\n");
        return NULL;
    }
    char * buff = malloc(sizeof(char)*300);
    strcpy(buff,"");
    if (fgets(buff,300,f) == NULL){
        printf("Le fichier .current_branch est vide\n");
        return NULL;
    }
    sscanf(buff,"%s\n",buff);
    return buff;
}