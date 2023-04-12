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
	int mode = getChmod("Test/Dossier");
	printf("Mode de Dossier -> %d\n",mode);
	mode = getChmod("Test/Dossier/Dfichier1.txt");
	printf("Mode de Test/Dossier/Dfichier1.txt -> %d\n",mode);

	
	
	WorkTree * WT1 = initWorkTree();
	char * h1 = sha256file("ctest1.c");
	char * h2 = sha256file("ctest2.c");
	//char * h3 = sha256file("teste");
	//appendWorkTree(WT1,"ctest1.c",h1,777);
	appendWorkTree(WT1,"ctest2.c",h2,777);
	appendWorkTree(WT1,"Test",NULL,getChmod("Test"));
	appendWorkTree(WT1,"ctest1.c",h1,777);
	afficheWT(WT1);
	char * h = saveWorkTree(WT1,".");
	printf(" isWorkTree -> %d\n",isWorkTree("ffeur"));	
	

	freeWorkTree(WT1);
	//free(h3);
	free(h1);
	free(h2);
	free(h);
	
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