#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>

//Done
int hashFile(char * source, char * dest){ //Ecrit le hash du fichier source dans le fichier destination
	char buff[1000];
	sprintf(buff,"cat %s | sha256sum > resultat.txt",source);	
	system(buff);
	FILE * temp = fopen("resultat.txt","r");
	fgets(buff,100,temp);
	buff[strlen(buff)-4] = '\0';
	FILE * d = fopen(dest,"w");
	fputs(buff,d);
	system("rm resultat.txt");
	fclose(temp);
	fclose(d);
	return 0;
}

//Done
char * sha256file(char * file){ //Renvoie le hash du fichier 
	//Cas où le fichier n'existe pas
	FILE * ftest;
	if ( ( ftest = fopen(file,"r")) == NULL){
		printf("Le fichier n'existe pas !(sha256file)\n");
		exit(EXIT_FAILURE);
	}
	else{
		fclose(ftest);
	}

	
	//Fichier temporaire
	char static fname[1000];
	sprintf(fname,"%sXXXXXX",file); //sans le /tmp/ à la maison et avec /tmp/ au début de la chaine à la fac j'imagine
	int fd = mkstemp(fname);
	
	//On hashe le fichier et on met ce hashage dans le fichier temporaire
	hashFile(file,fname);
	
	//On récupère le hash
	FILE* f = fopen(fname,"r");
	char * hash = malloc(1000*sizeof(char));
	fgets(hash,1000,f);

	//On supprime le fichier temporaire
	char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);
	fclose(f);
	
	return hash;
}

//done
int file_exists (char *file){ 
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

//done
int isFile(const char *path){
    struct stat path_stat;
    if (file_exists((char*)path) == 1){
        stat(path,&path_stat); 
        return S_ISREG(path_stat.st_mode);   //1 si file 0 si pas file -1 si n'existe pas 
    }
    return -1;
}
