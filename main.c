#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	WorkFile * WF1 = createWorkFile("main.txt"); 
	WF1->hash = sha256file(WF1->name);
	WF1->mode = 777;
	freeWorkFile(WF1);
	WorkTree * WT1 = initWorkTree();
	printf("%d\n",inWorkTree(WT1,"exo2.c"));
	char * h1=sha256file("exo1.c");
	char * h2=sha256file("exo2.c");
	char * h3=sha256file("exo3.c");
	printf("%d\n",WT1->n);
	appendWorkTree(WT1,"exo1.c",h1,777);
	printf("%d\n",WT1->n);
	appendWorkTree(WT1,"exo2.c",h2,777);
	printf("%d\n",WT1->n);
	free(h1);
	free(h2);
	free(h3);
	freeWorkFile(&(WT1->tab[0]));
	freeWorkFile(&(WT1->tab[1]));

	/*
	
	
	

	
	
	appendWorkTree(WT1,"exo3.c",h3,777);

	
	char * chaineWorkTree = wtts(WT1);
	printf("%s\n",chaineWorkTree);
	WorkTree* WT3 = stwt(chaineWorkTree);
	printf("%s",wtts(WT3));

	wttf(WT1,"fichier1.txt");
	WorkTree *WT4 = ftwt("fichier1.txt");
	printf("%s",wtts(WT4));
	
	freeWorkTree(WT1);

	
	
	*/
	return 0;
}


//Update : 0 leak pour l'instant sur l'exo4 (pas fini de tester toutes les fonctions) + freeWorktree provoque des erreurs en particulier freeWorkFile(&(WT1->tab[1]));