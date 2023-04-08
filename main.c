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

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	
	return 0;
}


/*
Update Simon : 0 leak jusqu'à stwf, reste pas testé + pb freeworkTree;
*/

/*
Update Nino : J'ai mis en commentaire un free dans hashToPath parce que ça enlève quelque
chose dont on a besoin -> NE JAMAIS FREE LES ARGUMENTS D'UNE FONCTION IL EXISTENT EN-DEHORS !!
Aussi saveWorkTree fonctionnel je crois (sûr à 80% dur de faire tout les tests)
Problème sur restoreWorkTree -> boucle infinie sur Test (test dans le main.txt + printf dans restoreWorkTree)
*/
