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
	system("./myGit init");
	system("./myGit add f1.txt");
	system("./myGit add f3.txt");
	system("./myGit commit master -m \"commit1 sur master\"");
	system("./myGit branch Branche");

	system("./myGit checkout-branch Branche");
	system("echo \"Branche 1\" > f1.txt");
	system("echo \"Branche 3\" > f3.txt");
	system("./myGit add f1.txt");
	system("./myGit add f2.txt");
	system("./myGit add f3.txt");
	system("./myGit commit Branche -m \"commit1 sur Branche\"");
	
	system("./myGit checkout-branch master");
	
	/*
	system("echo \"master 1\" > f1.txt");
	system("echo \"master 3\" > f3.txt");
	system("./myGit add f3.txt");
	system("./myGit add f1.txt");
	system("./myGit commit Branche -m \"commit2 sur Branche\"");
	*/
	
	
	WorkTree * bTW= branchToWorkTree("master");
	//afficheWT(bTW);
	freeWorkTree(bTW);

	List * Lc= merge("Branche","BRANCHEFUSION");
	if (Lc!=NULL){
		char * c = ltos(Lc);
		printf("conflicts : %s\n",c);
		createDeletionCommit("Branche",Lc,"commit de suppression des conflits");
		List * Lc2= merge("Branche","BRANCHEFUSION");
		free(c);
		FreeList(Lc);
		if (Lc2!=NULL){
			FreeList(Lc2);
			printf("erreur : conflits non résolus\n");
			exit(EXIT_FAILURE);
		}

	}
	
	return 0;
}


/*
Update Simon : 0 leak error jusqu'à createDeletionCommit inclus 
*/


/*
Update Nino :
-> merge n'est donc pas un pb, test dans main.txt
*/