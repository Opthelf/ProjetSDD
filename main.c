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
	
	printBranch("master");
	//system("rm -f -r */ ; rm -f -r .refs ; rm .current_branch  ");	


	
	

	return 0;
}


/*
Update Simon : 0 leak error jusqu'à createDeletionCommit inclus 
*/


/*
Update Nino :
-> branchList & getAllCommits sans problème, leak résolu (pas testé quand il y a des predecessor old)
*/