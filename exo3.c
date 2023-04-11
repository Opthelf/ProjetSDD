#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"

int octalVersDecimal(int octal){
    int decimal = 0;
    int i = 0;
    while(i*64 <= octal){
        i++;
    }
    octal = octal%64;
    decimal += 100*(i-1);
    i = 0;
    while(i*8 <= octal){
        i++;
    }
    octal = octal%8;
    decimal += 10*(i-1);
    i = 0;
    while(i <= octal){
        i++;
    }
    decimal += i-1;
    return decimal;
}

int getChmod(const char * path){
    struct stat ret;
    if(stat(path,&ret)==-1){
        return -1;
    }
    int octal = (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);
    return octalVersDecimal(octal);
}

void setMode(int mode, char * path){
    char buff[100];
    sprintf(buff,"chmod %d %s",mode,path);
    system(buff);
}

char * hashToFile(char *hash){
    char * ch2 = strdup(hash);
    ch2[2]='\0';
    struct stat st={0};
    if(stat(ch2,&st) == -1){
        mkdir(ch2,0700);
    }
    free(ch2);
    return hashToPath(hash);
}



List * listdir(char * root_dir){
	DIR *dp;
	struct dirent *ep;
	List * L = initList();
	Cell * temp_cell;
	dp = opendir(root_dir);
	if(dp != NULL){
		while((ep = readdir(dp)) != NULL){
			temp_cell = buildCell(ep->d_name);
			insertFirst(L,temp_cell);
		}
		(void)closedir(dp);
	}
	else{
		perror("On a pas pu ouvrir le repertoire\n");
		return NULL;
	}
	return L;
}

struct stat st = {0};

int file_exists (char *file){ 
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

char * hashToPath(char *hash){
	int l = strlen(hash);
	char * path = malloc(sizeof(char)*(l+2));
	path[0] = hash[0];
	path[1] = hash[1];
	path[2] = '/';
	int i = 3;
	int j = 2;
	while( hash[j]!='\0'){
		path[i] = hash[j];
		i++;
		j++;
	}
	path[i]='\0';
	return path;
}

void cp(char* to, char* from){
    if (file_exists(from) == 0){
        printf("Le fichier entré en paramètre n'est pas dans ce répertoire !(cp)\n");
        return;
    }
    FILE * f_depart = fopen(from,"r");
    FILE * f_arrivee = fopen(to,"w");
    char buff[256];
    while(fgets(buff,256,f_depart) != NULL){
        fprintf(f_arrivee,"%s",buff);
    }
    fclose(f_depart);
    fclose(f_arrivee);
}

void blobFile(char* file){
	char * hash = sha256file(file);
	char ch2[strlen(hash)]; 
	strcpy(ch2,hash);

	ch2[2]= '\0';
	if (!file_exists(ch2)){
		char buff[100];
		sprintf(buff,"mkdir %s",ch2);
		system(buff);
	}
	char * ch = hashToPath(hash);
	cp(ch,file);
	setMode(777,ch);

	free(ch);
	free(hash);
}