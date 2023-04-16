#ifndef GESTION_FILE_H
#define GESTION_FILE_H

int file_exists (char *file);
int isFile(const char *path);
int octalVersDecimal(int octal);
int getChmod(const char * path);
void setMode(int mode, char * path);
List * listdir(char * root_dir);
void cp(char* to, char* from);
void blobFile(char* file);
char * concat_paths(char * path1,char * path2);

#endif
