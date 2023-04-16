#ifndef HASH_H
#define HASH_H

int hashFile(char * source, char * dest);
char * sha256file(char * file);
char * hashToPath(char *hash);
char * hashToFile(char *hash);
char * hashToPathCommit(char * hash);
char * hashToPathWorkTree(char * hash);
unsigned long hash(unsigned char *str);

#endif
