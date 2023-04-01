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

<<<<<<< HEAD
int file_exists(char *file){
	
}
=======
>>>>>>> bf713722a3f04a443d8d884e7ee3c36dc4ae18ce
