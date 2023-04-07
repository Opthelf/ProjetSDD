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
#define TAILLE 10


WorkFile* createWorkFile(char* name){
    WorkFile * WF = (WorkFile *)malloc(sizeof(WorkFile));
    WF->name = strdup(name);
    WF->hash = NULL;
    WF->mode = 0;
    return WF;
}

void freeWorkFile(WorkFile * WF){
    if(WF==NULL){
        printf("WorkFile NULL(freeWorkFile)\n");
        free(WF);
        return;
    }
    if(WF->name!=NULL){
        free(WF->name);
    }
    if(WF->hash!=NULL){
        free(WF->hash);
    }
    free(WF);
}

char* wfts(WorkFile* wf){
    char * res = malloc(1000*sizeof(char));
    sprintf(res,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
    res[strlen(res)] = '\0';
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

void freeWorkTree(WorkTree * WT){
    for (int i = 0; i < WT->n; i++){
        free(WT->tab[i].name);
        free(WT->tab[i].hash);
        free(&(WT->tab[i]));
    }
    free(WT);
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
    //printf("Le fichier passé en paramètre n'est pas dans le WorkTree(inWorkTree)\n");
    return -1;
}

int appendWorkTree(WorkTree* wt,char * n,char * h, int m){
    if ( inWorkTree(wt,n) != -1){
        printf("Le fichier est déjà dans le WorkTree !\n");
        return 0;
    }
    if (wt->size > wt->n){
        wt->tab[wt->n].mode = m;
        wt->tab[wt->n].name = strdup(n);
        if(h != NULL){
            wt->tab[wt->n++].hash = strdup(h);
        }
        else{
            wt->tab[wt->n++].hash = NULL;
        }
        return 1;
    }
    return 0;
}


char* wtts(WorkTree* wt){
    int i = 0;
    char * res = (char*)malloc(sizeof(char)*1000);
    strcpy(res,wfts(&(wt->tab[i])));
    i++;
    while(i < wt->n-1){
        strcat(res,wfts(&(wt->tab[i])));
        strcat(res,"\n");
        i++;
    }
    strcat(res,wfts(&(wt->tab[i])));
    strcat(res,"\0");
    return res;
}

void afficheWorkTreeHash1(WorkTree* wt){
    int i = 0;
    while(i < wt->n){
        printf("%s %s\n",wt->tab[i].name,sha256file(wt->tab[i].name));
        i++;
    }
}
void afficheWorkTreeHash2(WorkTree* wt){
    int i = 0;
    while(i < wt->n){
        printf("%s %s\n",wt->tab[i].name,wt->tab[i].hash);
        i++;
    }
}

WorkTree* stwt(char* ch){
    WorkTree * WT= initWorkTree();
    char * resname = malloc(sizeof(char)*1000);
    char * reshash = malloc(sizeof(char)*1000);
    int resmode = 0;
    while(ch -1 != NULL){ //ch -1 et usage de strchr unclear mais fonctionnel en apparence ici
        sscanf(ch,"%s\t%s\t%d",resname,reshash,&resmode);
        strcat(resname,"\0");
        strcat(reshash,"\0");
        appendWorkTree(WT,resname,reshash,resmode);
        ch = strchr(ch,'\n')+1;
    }
    free(resname);
    free(reshash);
    return WT;

}

int wttf(WorkTree* wt, char* file){
    FILE * f = fopen(file,"w");
    if (f != NULL){
        char * chaine = wtts(wt);
        fprintf(f,"%s",chaine);
        fclose(f);
        return 1;
    }
    printf("Problème d'ouverture du fichier(wttf)\n");
    return 0;
}

WorkTree* ftwt(char* file){
    WorkTree * WT = initWorkTree();
    FILE * f = fopen(file,"r");
    char * buff = malloc(sizeof(char)*1000);
    char * chaineWT = malloc(sizeof(char)*1000*TAILLE);
    while(fgets(buff,1000,f) != NULL){
        strcat(chaineWT,buff);
    }
    WT = stwt(chaineWT);
    free(buff);
    free(chaineWT);
    return WT;
}