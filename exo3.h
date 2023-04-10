#ifndef exo3_H
#define exo3_H

int octalVersDecimal(int octal);
int getChmod(const char * path);
void setMode(int mode, char * path);
List * listdir(char * root_dir);
int file_exists(char *file);
void cp(char * to, char * from);
char * hashToPath(char* hash);
void blobFile(char * file);

#endif
