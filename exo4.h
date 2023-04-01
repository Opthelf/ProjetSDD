#ifndef EXO4_H
#define EXO4_H

typedef struct workfile { 
    char* name; 
    char* hash;
    int mode; 
}WorkFile;

typedef struct worktree { 
    WorkFile* tab;
    int size;
    int n;
}WorkTree;

WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkTree* initWorkTree();
int appendWorkTree(WorkTree* wt,char * n,char * h, int m);

#endif