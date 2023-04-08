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
    strcat(buff,"\0");
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
    return c;
}

void freeCommit(Commit *c){
    for(int i=0;i<c->size;i++){
        if(c->T[i]!=NULL){
            freeKeyVal(c->T[i]);
        }
    }
    free(c->T);
    free(c);
}

unsigned long   hash(unsigned char *str)
    {
        unsigned long hash = 5381;
        int c;

        while (c = *str++)
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
    }

void commitSet(Commit *c,char *key,char *val){
    int p=hash(key)%c->size;
    while(c->T[p]!=NULL){
        p=(p+1)%c->size;
    }
    c->T[p]=createKeyVal(key,val);
    c->n++;
}

Commit * createCommit(char* hash){
    Commit * c = initCommit();
    commitSet(c,"tree",hash);
    return c;
}

char * commitGet(Commit *c,char *key){
    int p=hash(key)%c->size;
    int attempt=0;
    while(c->T[p]!=NULL && attempt<c->size){
        if(strcmp(c->T[p]->key,key)==0){
            return c->T[p]->value;
        }
        p=(p+1)%c->size;
        attempt++;
    }
    return NULL;
}

char * cts(Commit *c){
    char * res = malloc(sizeof(char)*100*c->n);
    strcpy(res,"");
    for(int i=0;i<c->size;i++){
        if(c->T[i]!=NULL){
            char * temp=kvts(c->T[i]);
            strcat(res,temp);
            free(temp);
            if(c->T[i+1]!=NULL){
                strcat(res,"\n");
            }
        }
    }
    strcat(res,"\0");
    return res;
}

Commit * stc(char *str){
    Commit * c = initCommit();
    char * reskey = malloc(sizeof(char)*100);
    strcpy(reskey,"");
    char * resvalue = malloc(sizeof(char)*100);
    strcpy(resvalue,"");
    int i=0;
    while(str-1!=NULL){
        sscanf(str,"%s : %s",reskey,resvalue);
        strcat(reskey,"\0");
        strcat(resvalue,"\0");
        commitSet(c,reskey,resvalue);
        str=strchr(str,'\n')+1;
    }
    free(reskey);
    free(resvalue);
    return c;
}
void ctf(Commit *c,char *file){
    FILE *f = fopen(file,"w");
    if(f!=NULL){
        char * str = cts(c);
        fprintf(f,"%s",str);
        free(str);
        fclose(f);
    }
}
Commit * ftc(char *file){
    FILE *f = fopen(file,"r");
    if(f!=NULL){
        char * str = malloc(sizeof(char)*10000);
        strcpy(str,"");
        char * buff = malloc(sizeof(char)*100);
        strcpy(buff,"");
        while(fgets(buff,100,f)!=NULL){
            strcat(str,buff);
        }
        free(buff);
        fclose(f);
        Commit * c = stc(str);
        free(str);
        return c;
    }
}

char * blobCommit(Commit *c){
    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);
    ctf(c,fname);
    char * hash = sha256file(fname);
    char * ch = hashToFile(hash);
    strcat(ch,".c");
    cp(ch,fname);
    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);
    free(ch);
    return hash;

}

