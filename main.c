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
	FILE * f1 = fopen("Test/fichier1.txt","w");
	if (f1 == NULL){
		printf("Error %d \n", errno);
		return 0;
	}
	FILE * f2 = fopen("Test/fichier2.txt","w");
	if (f2 == NULL){
		printf("Error %d \n", errno);
		return 0;
	}
	fprintf(f1,"Premier fichier\n");
	fprintf(f2,"Deuxième fichier\n");
	fclose(f1);
	fclose(f2);

	WorkTree * WT1 = initWorkTree();
	appendWorkTree(WT1,"Test",NULL,getChmod("Test"));
	char * workTreeHash = saveWorkTree(WT1,".");

	Commit * c1 = initCommit();
	commitSet(c1,"author","Nino");
	commitSet(c1,"message","Sauvegarde Test");
	commitSet(c1,"tree",workTreeHash);
	commitSet(c1,"predecessor","null");

	initRefs();
	createUpdateRef("HEAD",commitGet(c1,"tree"));

	/*createUpdateRef("branche1",commitGet(c1,"tree"));
	deleteRef("branche1");*/

	/*char * hash = getRef("branche1");
	char * hash1 = getRef("HEAD");
	char * hash2 = getRef("master");
	printf("hash1 : %s\n",hash1);
	printf("hash2 : %s\n",hash2);
	free(hash);
	free(hash1);
	free(hash2);*/
	myGitAdd("exo7.c");
	myGitAdd("exo6.c");
	myGitCommit("Head","Test de myGitCommit");
	char * gr = getRef("HEAD");
	printf("gr : %s\n",gr);
	freeWorkTree(WT1);
	freeCommit(c1);
	free(workTreeHash);
	free(gr);

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