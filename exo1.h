#ifndef EXO1_H
#define EXO1_H

int hashFile(char * source, char * dest);
char * sha256file(char * file);
int file_exists(char *file);
int isFile(const char * path); //https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file

#endif
