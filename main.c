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
<<<<<<< HEAD

	//Bloc 1 :
	List * L1 = initList();
	Cell * C1 = buildCell("Chaine1");
	Cell * C2 = buildCell("Chaine2");
	Cell * C3 = buildCell("Chaine3");
	insertFirst(L1,C1);
	insertFirst(L1,C2);
	insertFirst(L1,C3);
	char * recup = ctos(C1);
	//char * recupT = ltos(L1);
	Cell * C4 = listGet(L1,0);
	Cell * C5 = searchList(L1,"Chaine1");
	List * L2 = stol("Je|suis|heureux");
	printf("%s\n",ltos(stol(ltos(L2)))); //Utiliser deux fois sinon à l'envers
	//ltof(L1,"TestLtof.txt");
	//List * L3 = ftol("TestLtof.txt");
	
	//Bloc 2 :
	char cwd[50];
	List * L4 = listdir(".");
	//printf("%s\n",ltos(stol(ltos(L4))));
	//printf("%s\n",hashToPath(sha256file("main.c")));
	cp("fichier2.txt","fichier1.txt");
	blobFile("exo1.c");

	//Bloc 3 :

	WorkFile * WF1 = createWorkFile("exo1.c"); 
	WF1->hash = sha256file(WF1->name); //Sur mac ne marche pas avec un fichier txt
	WF1->mode = 777;
	//freeWorkFile(WF1);
	WorkFile * WF2 = createWorkFile("exo2.c"); 
	WF2->hash = sha256file(WF2->name); //Sur mac ne marche pas avec un fichier txt
	WF2->mode = 777;
	//freeWorkFile(WF2);
	char * chaineWorkFile = wfts(WF1);
	//WF1 = stwf(chaineWorkFile);

	WorkTree * WT1 = initWorkTree();
	//printf("%d\n",inWorkTree(WT1,"exo2.c"));

	appendWorkTree(WT1,"exo3.c",sha256file("exo3.c"),777);
	appendWorkTree(WT1,"exo1.c",sha256file("exo1.c"),777);
	appendWorkTree(WT1,"exo2.c",sha256file("exo2.c"),777);
	
	//printf("%s\n",WT1->tab[WT1->n-2].hash);
	//printf("%s\n",WT1->tab[WT1->n-2].name);
	//printf("%d\n",WT1->n);

	char * chaineWorkTree = wtts(WT1);
	printf("%s\n",chaineWorkTree);
	WorkTree* WT3 = stwt(chaineWorkTree);
	//printf("%s",wtts(WT3));
	//wttf(WT1,"fichier1.txt");
	//WorkTree *WT4 = ftwt("fichier1.txt");
	//printf("%s",wtts(WT4));
	/*freeWorkTree(WT1);
	freeWorkFile(WF1);
	freeWorkFile(WF2);*/


	//BLoc 4 :
	//printf("%d\n",getChmod("fichier1.txt"));
	//printf("%s\n",sha256file("exo1.c"));
	//printf("%s\n",hashToFile(sha256file("exo1.c")));
	//printf("%s\n",blobWorkTree(WT1));
	/*printf("%d\n",isFile("Test"));
	printf("%d\n",isFile("exo1.c"));
	printf("%s\n",saveWorkTree(WT1,"."));*/


	//Bloc 5 :



=======
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
>>>>>>> 1aebce10d9e57d1370d91aa005e48f57f8fc88bf
	return 0;
}


//Update : Bloc3 terminé mais leak de mémoire (à voir plus tard).