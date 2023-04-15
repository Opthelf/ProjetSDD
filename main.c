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
	system("echo \"master 1\" > f1.txt");
	system("echo \"master 3\" > f3.txt");
	system("echo \"master 4\" > f4.txt");
	system("./myGit add f1.txt");
	system("./myGit add f3.txt");
	system("./myGit add f4.txt");
	system("./myGit commit master -m \"commit1 sur master\"");
	
	system("./myGit branch Branche");
	system("./myGit checkout-branch Branche");
	
	system("echo \"Branche 1\" > f1.txt");
	/*
	
	system("echo \"Branche 3\" > f3.txt");
	system("echo \"Branche 4\" > f4.txt");*/

	
	system("./myGit add f4.txt");
	system("./myGit add f3.txt");
	system("./myGit add f1.txt");
	system("./myGit add f2.txt");
	system("./myGit commit Branche -m \"commit1 sur Branche\"");
	
	system("./myGit checkout-branch master");

	system("./myGit get-current-branch");

	List * conflicts = merge("Branche","fusion master et branche");
	
	if(conflicts != NULL){
		char * str = ltos(conflicts);
		printf("\nconflicts : %s\n",str);
		free(str);
		FreeList(conflicts);
	}
	

	
	/*createDeletionCommit("Branche",conflicts,"commit suppression conflit ");
    merge("Branche","fusion master et branche");*/

	//system("rm -f -r */ ; rm -f -r .refs ; rm .current_branch  ");
	//	//FreeList(conflicts2);
	return 0;
}


/*
Update Simon : 0 leak error jusqu'à createDeletionCommit inclus 
*/


/*
Update Nino :
-> merge n'est donc pas un pb, test dans main.txt
*/