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
	/*List * L1 = initList();
	List * L2 = initList();

	Cell * C1 = buildCell("first");
	Cell * C2 = buildCell("second");
	Cell * C3 = buildCell("third");

	insertLast(L1,C1);
	insertLast(L1,C2);
	insertLast(L1,C3);

	Cell * C4 = buildCell("forth");
	Cell * C5 = buildCell("fifth");
	Cell * C6 = buildCell("sixth");

	insertLast(L2,C4);
	insertLast(L2,C5);
	insertLast(L2,C6);

	char * chaine_1 = ltos(L1);
	printf("chaine 1 -> %s\n",chaine_1);

	char * chaine_2 = ltos(L2);
	printf("chaine 2 -> %s\n",chaine_2);

	fusionList(&L1,L2);
	char * chaine_3 = ltos(L1);
	printf("chaine 3 -> %s\n",chaine_3);

	free(chaine_1);
	free(chaine_2);
	free(chaine_3);

	FreeList(L1);
	free(L2);*/

	//char * chaine = getRef("master");
	List * commits = getAllCommits();
	char * chaine_commits = ltos(commits);

	printf("commits ->\n%s\n",chaine_commits);

	free(chaine_commits);
	//free(chaine);
	FreeList(commits);
	return 0;
}


/*
Update Simon : 0 leak error jusqu'à createDeletionCommit inclus 
*/


/*
Update Nino :
-> merge n'est donc pas un pb, test dans main.txt
*/