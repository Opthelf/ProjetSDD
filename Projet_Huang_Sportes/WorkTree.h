#ifndef WORKTREE_H
#define WORKTREE_H

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
char * blobWorkTree(WorkTree *wt);
char * saveWorkTree(WorkTree *wt,char * path);
int isWorkTree(char* hash);
void restoreWorkTree(WorkTree * wt, char * path);
WorkTree * branchToWorkTree(char * branch_name);

#endif
