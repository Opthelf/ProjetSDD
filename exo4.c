#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
# define TAILLE 10


WorkFile* createWorkFile(char* name){
    WorkFile * WF = (WorkFile *)malloc(sizeof(WorkFile));
    WF->name = strdup(name);
    WF->hash = NULL;
    WF->mode = 0;
    return WF;
}

char* wfts(WorkFile* wf){
    char * res = malloc(1000*sizeof(char));
    sprintf(res,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
    return res;
}

WorkFile* stwf(char* ch){
    int mode;
    char * name = malloc(sizeof(char)*1000);
    char * hash = malloc(sizeof(char)*1000);
    sscanf(ch,"%s\t%s\t%d",name,hash,&mode);
    WorkFile * WF = createWorkFile(name);
    WF->hash = hash;
    WF->mode = mode;
    return WF;
}

WorkTree * initWorkTree(){
    WorkTree * WT = (WorkTree *)malloc(sizeof(WorkTree));
    WT->size = TAILLE;
    WT->tab = (WorkFile *)malloc(TAILLE*sizeof(WorkFile));
    WT->n = 0;
    return WT;
}

int inWorkTree(WorkTree* wt, char* name){
    if (wt == NULL){
        printf("Le WorkTree est NULL(inWorkTree)\n");
        return 0;
    }
    WorkFile * WF = wt->tab;
    int i = 0;
    while(i < (wt->n)){
        if (strcmp(name,WF[i].name) == 0){
            return i;
        }
        i++;
    }
    printf("Le fichier passé en paramètre n'est pas dans le WorkTree\n");
    return -1;
}

/*int appendWorkTree(WorkTree* wt,char * n,char * h, int m){
    WorkFile *WF = createWorkFile(n);
    WF->hash = strdup(hash);
    WF->mode = mode;
    if(wt->n == wt->size){
        printf("Worktree plein (appendWorkTree)\n");
        return 0;
    }
    for(int i=0;i<wt->n;i++){
        if(strcmp(wt->tab[i]->name,name)==0 && strcmp(wt->tab[i]->hash,hash)==0 && wt->tab[i]->mode == mode){
            printf("Fichier déjà présent (appendWorkTree)\n");
            return 0;
        }
    }
    (wt->tab[wt->n]) = WF ;
    wt->n++;
    printf("Fichier ajouté (appendWorkTree)\n");
    return 1;
}
*/