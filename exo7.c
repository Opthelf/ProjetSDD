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
    char * recup = malloc(sizeof(char)*256);
    strcpy(recup,"");
    if (fgets(recup,256,f) == NULL){
        printf("Le fichier %s est vide !(getRef)\n",buff);
    }
    return recup;
}

void myGitAdd(char* file_or_folder){
    WorkTree * wt;
    if (file_exists(".add") == 0){
        system("touch .add");
        wt = initWorkTree();
    }
    else{
        wt = ftwt(".add");
    }
    if (file_exists(file_or_folder) != 0){
        appendWorkTree(wt,file_or_folder,NULL,getChmod(file_or_folder));
        wttf(wt,".add");
    }
    else{
        printf("Le fichier ou répertoire %s n'existe pas !(myGitAdd)",file_or_folder);
    }
    //freeWorkTree(wt); Il faudra enlever le commentaire mais freeWorkTree ne marche pas :/
}

void myGitCommit(char* branch_name, char* message){
    if (file_exists(".refs") == 0){
        printf("Initialiser d'abord les références du projet -> (myGitCommit)\n");
        return;
    }
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",branch_name);
    if (file_exists(buff) == 0){
        printf("La branche %s n'existe pas -> (myGitCommit)\n",branch_name);
        return;
    }
    char * hashHEAD = getRef("Head");
    char * hashBranch_name = getRef(branch_name);
    if (strcmp(hashHEAD,hashBranch_name) != 0){
        printf("HEAD doit pointer sur le dernier commit de la branche -> (myGitCommit)\n");
        return;
    }
    WorkTree * WT = ftwt(".add");
    system("rm .add");
    if (WT == NULL){
        printf("Aucun fichier n'étaient présent dans le fichier .add -> (myGitCommit)\n");
        return;
    }
    char * hashWorkTree = saveWorkTree(WT,".");
    Commit * c = createCommit(hashWorkTree);
    if (strlen(hashBranch_name) == 0){
        printf("Le hash dans le fichier %s est vide -> (myGitCommit)\n",branch_name);
    }
    else{
        commitSet(c,"predecessor",hashBranch_name);
    }
    if (message == NULL){
        printf("Le message est NULL -> (myGitCommit)\n");
    }
    else{
        commitSet(c,"message",message);
    }
    char * hashCommit = blobCommit(c);
    createUpdateRef("Head",hashCommit);
    createUpdateRef(branch_name,hashCommit);

    free(hashHEAD);
    free(hashBranch_name);
    free(hashWorkTree);
    free(hashCommit);
}