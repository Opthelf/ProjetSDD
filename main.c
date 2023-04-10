#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	int o = octalVersDecimal(511);
	printf("octalVersDecimal(777) = %d\n",o);
	int gcm = getChmod("exo5.c");
	printf("getChmod(\"exo5.c\") = %d\n",gcm);
	setMode(420,"exo5.c");
	gcm = getChmod("exo5.c");
	printf("getChmod(\"exo5.c\") = %d\n",gcm);
	char * hash = sha256file("exo5.c");
	printf("sha256file(\"exo5.c\") = %s\n",hash);
	
	char * htf = hashToFile(hash);
	printf("hashToFile(\"exo5.c\") = %s\n",htf);
	
	WorkTree * WT= initWorkTree();
	appendWorkTree(WT,"exo4.c",hash,777);
	appendWorkTree(WT,"exo3.c",hash,777); 
	char * blob = blobWorkTree(WT);
	printf("blobWorkTree(\"exo5.c\") = %s\n",blob);

	char * path =concat_paths("exo4.c","exo5.c");
	printf("concat_paths(\"exo4.c\",\"exo5.c\") = %s\n",path);
	
	printf("isFile(\"exo5.c\") = %d\n",isFile("exo5.c"));
	printf("isFile(\"Test\") = %d\n",isFile("Test"));
	printf("isFile(\"exo10.c\") = %d\n",isFile("exo10.c"));	
	
	char * save= saveWorkTree(WT,".");    
	
	free(path);
	free(blob);
	free(hash);
	free(htf);
	freeWorkTree(WT);
	free(save);
	
	return 0;
}


/*
*/
//	ex6 terminé tout marche (normalement) mais erreur du type :Invalid write of size n sur blobCommit à régler ) ;

