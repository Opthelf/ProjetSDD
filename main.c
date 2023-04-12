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
	WorkFile * WF1 = createWorkFile("Test");
	WorkFile * WF2 = createWorkFile("test.txt");
	WorkTree * WT = initWorkTree();
	appendWorkTree(WT,WF1->name,WF1->hash,WF1->mode);
	appendWorkTree(WT,WF2->name,WF2->hash,WF2->mode);
	char * chaine = wtts(WT);
	printf("chaine -> \n%s\n",chaine);
	char * hash = saveWorkTree(WT,".");

	freeWorkFile(WF1);
	freeWorkFile(WF2);
	freeWorkTree(WT);
	free(chaine);
	free(hash);
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