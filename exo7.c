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

//La fonction crée le répertoire .refs et les fichiers master et HEAD dedans si ce n'est pas déjà fait
void initRefs(){ 

    //Si le dossier n'existe pas déjà
    if(!file_exists(".refs")){
        system("mkdir .refs");    
        system("echo null > .refs/master");
        system("echo null > .refs/HEAD");
    }
}

//La fonction update une référence en y insérant un nouveau hash
void createUpdateRef(char* ref_name, char* hash){

    //Si le hash est NULL
    if (hash == NULL){ 
        printf("Le hash est NULL -> createUpdateRef\n");
        exit(EXIT_FAILURE);
    }

    //Insère le hash dans la ref
    char buff[300];
    char buff1[300];
    strcpy(buff,"");
    strcpy(buff1,"");
    sprintf(buff,"echo %s > .refs/%s",hash,ref_name);
    sprintf(buff1,"echo %s > .refs/HEAD",hash);
    system(buff);
    system(buff1);
}

//La fonction efface une référence
void deleteRef(char* ref_name){

    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);

    //Si le fichier n'existe pas
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(deleteRef)\n",buff);
        exit(EXIT_FAILURE);
    }

    sprintf(buff,"rm .refs/%s",ref_name);
    system(buff);
}

//Récupère le hash d'une référence
char* getRef(char* ref_name){

    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);
    printf("ici\n");

    //Si le fichier n'existe pas
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(getRef)\n",buff);
        return NULL;
    }

    FILE * f = fopen(buff,"r");

    //Si il y a eu un problème d'ouverture
    if (f == NULL){
        printf("Erreur %d d'ouverture lors du fichier(getRef)\n",errno);
        return NULL;
    }

    char * recup = malloc(sizeof(char)*300);
    strcpy(recup,"");

    //On récupère le hash dans le fichier
    if (fgets(recup,256,f) == NULL){
        //printf("Le fichier %s est vide !(getRef)\n",buff);
        return NULL;
    }

    sscanf(recup,"%s\n",recup);
    fclose(f);
    return recup;
}

//Ajoute le fichier/dossier dans le fichier .add sous forme de WorkFile/WorkTree
void myGitAdd(char* file_or_folder){
    WorkTree * wt;

    //Si le fichier .add n'existe pas
    if (file_exists(".add") == 0){
        system("touch .add");
        wt = initWorkTree();
    }

    //Si il existe on récupère directement le WorkTree à l'intérieur
    else{
        wt = ftwt(".add");
    }

    //Si le fichier existe on le rajoute au WorkTree
    if (file_exists(file_or_folder) != 0){
        appendWorkTree(wt,file_or_folder,NULL,getChmod(file_or_folder));
        wttf(wt,".add");
    }

    //Sinon on le signale
    else{
        printf("Le fichier ou répertoire %s n'existe pas !(myGitAdd)\n",file_or_folder);
    }
    freeWorkTree(wt);
}


//Commit dans une branche
void myGitCommit(char* branch_name, char* message){

    //Si le fichier .refs n'existe pas
    if (file_exists(".refs") == 0){
        printf("Initialiser d'abord les références du projet -> (myGitCommit)\n");
        exit(EXIT_FAILURE);
    }

    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",branch_name);

    //Si la référence n'existe pas
    if (file_exists(buff) == 0){
        printf("La branche %s n'existe pas -> (myGitCommit)\n",branch_name);
        exit(EXIT_FAILURE);
    }

    char * hashHEAD = getRef("HEAD");
    char * hashBranch_name = getRef(branch_name);

    //Si la ref HEAD ne pointe pas sur le même commit que la branche
    if (strcmp(hashHEAD,hashBranch_name) != 0){
        printf("HEAD doit pointer sur le dernier commit de la branche -> (myGitCommit)\n");
        free(hashHEAD);
        free(hashBranch_name);
        return;
    }

    WorkTree * WT = ftwt(".add");
    system("rm .add");

    //Si le WorkTree récupéré du fichier .add est NULL
    if (WT == NULL){
        free(hashHEAD);
        free(hashBranch_name);
        printf("Aucun fichier n'étaient présent dans le fichier .add -> (myGitCommit)\n");
        return;
    }

    //On save le WorkTree récupéré
    char * hashWorkTree = saveWorkTree(WT,".");
    Commit * c = createCommit(hashWorkTree);

    //Si la référence est une chaine de caractère vide
    if (strlen(hashBranch_name) == 0){
        printf("Le hash dans le fichier %s est vide -> (myGitCommit)\n",branch_name);
    }

    //Sinon on ajoute une paire predecessor-hash au commit
    else{
        printf("hashBranch_name -> %s\n",hashBranch_name);
        commitSet(c,"predecessor",hashBranch_name);
    }

    //Si le message est NULL on le signale simplement
    if (message == NULL){
        printf("Le message est NULL -> (myGitCommit)\n");
    }

    //Si le message n'est pas NULL on ajoute une paire message-"message"
    else{
        commitSet(c,"message",message);
    }

    //On crée un instantané du commit
    char * hashCommit = blobCommit(c);
    printf("hashCommit -> %s\n",hashCommit);

    //On update la référence branch_name
    createUpdateRef(branch_name,hashCommit);
    createUpdateRef("HEAD",hashCommit);

    free(hashHEAD);
    free(hashBranch_name);
    free(hashWorkTree);
    free(hashCommit);
    freeWorkTree(WT);
    freeCommit(c);
}