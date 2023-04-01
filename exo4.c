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