#ifndef EXO5_H
#define EXO5_H
#include "exo4.h"

char * hashToFile(char* hash); // Correction
char * blobWorkTree(WorkTree *wt); //Correction
int isFile(const char * path); //https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
char * saveWorkTree(WorkTree *wt,char * path); //Correction
char * concat_paths(char * path1,char * path2); //Correction

#endif  
