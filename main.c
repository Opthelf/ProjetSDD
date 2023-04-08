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
#include "exo6.h"

int main(int argc, char ** argv){

	return 0;
}


//Update Simon : 0 leak jusqu'à stwf, reste pas testé + pb freeworkTree
//	ex6 terminé tout marche (normalement) mais erreur du type :Invalid write of size n sur blobCommit à régler ) ;

//Update Nino : J'ai mis en commentaire un free dans hashToPath parce que ça enlève quelque
//chose dont on a besoin -> NE JAMAIS FREE LES ARGUMENTS D'UNE FONCTION IL EXISTENT EN-DEHORS !!