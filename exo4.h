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
void freeWorkFile(WorkFile * WF);

WorkTree* initWorkTree();
void freeWorkTree(WorkTree* wt);

char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);

int inWorkTree(WorkTree* wt, char* name);
int appendWorkTree(WorkTree* wt,char * n,char * h, int m);
void afficheWT(WorkTree * wt);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* ch);
WorkTree * ftwt(char * file);


int wttf(WorkTree* wt, char* file);



#endif