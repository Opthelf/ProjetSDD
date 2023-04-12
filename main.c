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
#include "exo8.h"

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	int o = octalVersDecimal(511);
	printf("octalVersDecimal(777) = %d\n",o);
	int gcm = getChmod("exo5.c");
	printf("getChmod(\"exo5.c\") = %d\n",gcm);
	setMode(720,"exo5.c");
	gcm = getChmod("exo5.c");
	printf("getChmod(\"exo5.c\") = %d\n",gcm);
	char * hash = sha256file("exo5.c");
	printf("sha256file(\"exo5.c\") = %s\n",hash);
	char * h1=sha256file("exo4.c");
	char * h2=sha256file("exo3.c");
	
	char * htf = hashToFile(hash);
	printf("hashToFile(\"exo5.c\") = %s\n",htf);
	
	WorkTree * WT= initWorkTree();
	appendWorkTree(WT,"exo4.c",h1,777);
	appendWorkTree(WT,"exo3.c",h2,777); 
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
	free(h1);
	free(h2);
	return 0;
}


/*
Update Simon : ex6 terminé plus de leak mais tu peux tjrs faire plus de tests ;
exo8 terminé mais rien n'est testé ,en tout cas pour les fonctions que j'ai faites (à partir de printBranch)
*/


/*
Update Nino -> restoreWorkTree fonctionne !!! demande pour tester c'est quand même particulier
-> exo8 commencé, pour l'instant OK, mais petit doute sur le fonctionnement de myGitCommit, à rechecker 

Exo 1 à 4 revu, solidifié le code avec des tests, commentaires brefs pour expliquer ce que fait la fonction et testé jusqu'au milieu de l'exo 4 à peu près
*/

/* Leak -> appendWorkTree, saveWorkTree, restoreWorkTree, blobCommit
*/