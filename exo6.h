#ifndef EXO6_H
#define EXO6_H

typedef struct key_value_pair{
    char *key;
    char *value;
}kvp;

typedef struct hash_table{
    kvp ** T;
    int n;
    int size;
}HashTable;

typedef HashTable Commit;

kvp* createKeyVal(char *key,char *val);
void freeKeyVal(kvp *kv);
char * kvts(kvp* k);
kvp * stkv(char* str);
Commit* initCommit();
void freeCommit(Commit *c);
unsigned long hash(unsigned char *str);
void commitSet(Commit *c,char *key,char *val);
Commit * createCommit(char* hash);
char * commitGet(Commit *c,char *key);
char * cts(Commit *c);
Commit * stc(char *str);





#endif