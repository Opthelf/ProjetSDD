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
#include "exo9.h"
#include "exo10.h"
#include "exo11.h"
#include <stdlib.h>

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	/*initRefs();
	initBranch();*/

	//createBranch("branche_test");
	//myGitCheckoutBranch("master");
	/*myGitAdd("test.txt");
	myGitCommit("master","nouvelleBranche");*/

	List * L = getAllCommits();
	char * chaine_commit = ltos(L);
	printf("chaine ->\n%s\n",chaine_commit);

	free(chaine_commit);
	FreeList(L);


	/*createBranch("Branche1");
	printBranch("Branche1");
	List * L = branchList("Branche1");
	char * Lb1= ltos(L);
	printf("Liste de Branche1 : %s\n",Lb1);
	FreeList(L);
	free(Lb1);

	List * L3 = getAllCommits();
	
	char * Lb3= ltos(L3);
	printf("Liste de tous les commits : %s\n",Lb3);
	
	free(Lb3);
	FreeList(L3);*/
	
	/*
	
	List * L2 = branchList("master");
	
	char * Lb2= ltos(L2);
	
	printf("Liste de master : %s\n",Lb2);
	
	printBranch("master");
	
	FreeList(L2);
	
	free(Lb2);
	*/
	
	//free(refH);
	//free(refm);
	return 0;
}


/*
Update Simon : tous les exos jusqu'à 8 inclus sont 0 leaks à priori mais message pas bien traité cad que si le message contient des espaces alors seul le premier mot sera gardé pb venant de stc .

*/


/*
Update Nino :
-> exo 7, myGitCommit changé pour tester lorsque c'est le premier commit et donc qu'il n'y a rien dans HEAD et la branche à commit, semble fonctionner maintenant (sans leak)
-> exo 11, fonction mergeWorkTrees testé un petit peu, semble fonctionner
-> exo 11, fonction merge écrite avec attention, ajout de fonctions intermédiaires, mais non-testé
-> exo 11, fonction branchToWorkTree testé et fonctionnelle sur un test dans main.txt (dépend de ce que tu as dans ta branche) 
*/