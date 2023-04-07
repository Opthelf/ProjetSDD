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
	WorkTree * WT1 = initWorkTree();
	appendWorkTree(WT1,"Test",NULL,777);
	//printf("%d\n",WT1->tab[0].mode);
	//printf("%s\n",ltos(listdir("/Users/Nino/Documents/cours/2022-2023-s2/LU2IN006/Projet/ProjetSDD1/Test")));
	//printf("%d\n",getChmod("fichier1.txt"));
	//printf("%s\n",sha256file("exo1.c"));
	//printf("%s\n",hashToFile(sha256file("exo1.c")));
	//printf("%s\n",blobWorkTree(WT1));
	printf("%s\n",saveWorkTree(WT1,"."));
	return 0;
}


//Update Simon : 0 leak jusqu'à stwf, reste pas testé + pb freeworkTree;

//Update Nino : J'ai mis en commentaire un free dans hashToPath parce que ça enlève quelque
//chose dont on a besoin -> NE JAMAIS FREE LES ARGUMENTS D'UNE FONCTION IL EXISTENT EN-DEHORS !!