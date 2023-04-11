#ifndef EXO5_H
#define EXO5_H

char * hashToFile(char* hash); // Correction
char * blobWorkTree(WorkTree *wt); //Correction
char * concat_paths(char * path1,char * path2); //Correction
char * saveWorkTree(WorkTree *wt,char * path); //Correction
int isWorkTree(char* hash);
void restoreWorkTree(WorkTree * wt, char * path);


#endif
