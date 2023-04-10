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

#include <stdlib.h>


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
	char * h1=sha256file("exo4.c");
	char * h2=sha256file("exo3.c");
	
	char * htf = hashToFile(hash);
	printf("hashToFile(\"exo5.c\") = %s\n",htf);
	
	WorkTree * WT= initWorkTree();
	appendWorkTree(WT,"exo6.c",h1,777);
	appendWorkTree(WT,"exo3.c",h2,777); 
	char * blob = blobWorkTree(WT);
	freeWorkTree(WT);
	
	WorkTree * WT2= initWorkTree();
	char * save= saveWorkTree(WT2,"."); 
	appendWorkTree(WT2,"exo6.c",h1,777);
	char * save2= saveWorkTree(WT2,"."); 
	char * path =concat_paths("exo4.c","exo5.c");
	printf("blobWorkTree(\"exo5.c\") = %s\n",blob);
	  
	free(save);
	free(blob);
	free(save2);
	free(path);
	freeWorkTree(WT2);
	free(hash);
	free(htf);
	
	
	free(h1);
	free(h2);

	return 0;
}


/*
*/
//	ex6 terminé tout marche (normalement) mais erreur du type :Invalid write of size n sur blobCommit à régler ) ;

