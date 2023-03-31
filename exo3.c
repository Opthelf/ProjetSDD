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
