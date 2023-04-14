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

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici


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