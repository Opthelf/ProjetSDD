#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"

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

int file_exists(char * file){
	List * L = listdir(".");
	Cell * temp = *L;
	while (temp != NULL){
		if (strcmp(file,temp->data) == 0){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void cp(char *to, char *from){
	if (file_exists(from) == 0){
		printf("Le fichier entré en paramètre n'est pas dans ce répertoire !(cp)\n");
		return;
	}
	FILE * 
}