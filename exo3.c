#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
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

void afficherRep(char * root_dir){
	DIR *dp;
	struct dirent *ep;
	dp = opendir(root_dir);
	if(dp != NULL){
		while((ep = readdir(dp)) != NULL){
			char * temp = strdup(ep->d_name);
			printf("%s\n ",temp);
		}
		(void)closedir(dp);
	}
	else{
		perror("On a pas pu ouvrir le repertoire\n");
	}
}	

void afficheL(List * L){
	if (*L == NULL){
		printf("Liste NULL(afficheL)\n");
	}
	Cell * temp = *L;
	while( temp != NULL ){
		printf("%s\n",ctos(temp));
		temp = temp->next;
	}
}