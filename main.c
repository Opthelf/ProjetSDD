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

#include <stdlib.h>


int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici
	return 0;
}


/*
Update Simon : 0 leak et erreur exo5 jusqu'à isFile inclus
*/
//	ex6 terminé tout marche (normalement) mais erreur du type :Invalid write of size n sur blobCommit à régler ) ;


/*
Update Nino : Supprime mes updates ou mets les dans remarque.txt quand tu les vois (ce message aussi), je ferai la même avec les tiens si ça te va
Generale : Je pense qu'il faudra refaire wttf ou/et ftwt, au moins des tests y a des fois y a des comportements bizarres
Exo 5 : Problème sur restoreWorkTree -> boucle infinie sur Test (test dans le main.txt + printf dans restoreWorkTree)
Exo 7 : createUpdateRef, deleteRef, getRef, mtGitAdd, myGitCommit testés et fonctionnels (sans rien qui alerte mais les deux dernières sont longues donc faut voir) 
+ fichier myGit.c ajouté et testé
*/

