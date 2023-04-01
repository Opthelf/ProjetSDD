#ifndef exo3_H
#define exo3_H

List * listdir(char * root_dir);
int file_exists(char *file);
void afficherRep(char * root_dir);
void afficheL(List * L);
void cp(char * to, char * from);
char * hashToPath(char* hash);
void blobFile(char * file);

#endif
