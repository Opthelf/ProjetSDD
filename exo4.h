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


#endif