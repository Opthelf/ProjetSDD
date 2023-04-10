#ifndef EXO5_H
#define EXO5_H

char * hashToFile(char* hash); // Correction
char * blobWorkTree(WorkTree *wt); //Correction
int isFile(const char * path); //https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
char * concat_paths(char * path1,char * path2); //Correction
char * saveWorkTree(WorkTree *wt,char * path); //Correction
int isWorkTree(char* hash);
void restoreWorkTree(WorkTree * wt, char * path);


#endif
