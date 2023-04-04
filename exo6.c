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
#include "exo6.h"
#define TAILLE 10

kvp * createKeyVal(char *key,char *val){
    kvp * k =malloc(sizeof(kvp));
    k->key = strdup(key);
    k->value = strdup(val);
    return k;
}

void freeKeyVal(kvp *kv){
    free(kv->key);
    free(kv->value);
    free(kv);
}

char * kvts(kvp* k){
    char *buff = malloc(sizeof(char)*100);
    sprintf(buff,"%s : %s",k->key,k->value);
    return buff;
}

kvp *stkv(char*ch){
    char key[100],val[100];
    sscanf(ch,"%s : %s",key,val);
    return createKeyVal(key,val);
}

Commit* initCommit(){
    Commit *c=malloc(sizeof(Commit));
    c->T=malloc(sizeof(kvp*)*TAILLE);
    c->size = TAILLE;
    for(int i=0;i<c->size;i++){
        c->T[i]=NULL;
    }
    c->n=0;
}