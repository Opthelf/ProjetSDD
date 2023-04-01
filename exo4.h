#ifndef EXO4_H
#define EXO4_H

typedef struct { 
    char* name; 
    char* hash;
    int mode; 
}WorkFile;

typedef struct { 
    WorkFile* tab;
    int size;
    int n;
}WorkTree;

WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);
WorkTree * initWorkTree();
#endif