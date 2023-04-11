#ifndef exo3_H
#define exo3_H

int octalVersDecimal(int octal);
int getChmod(const char * path);
void setMode(int mode, char * path);
char * hashToPath(char* hash);
char * hashToFile(char *hash);
List * listdir(char * root_dir);
void cp(char * to, char * from);
void blobFile(char * file);

#endif
