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
	char * hash = sha256file("exosdqdqs1.c");
	free(hash);
	return 0;
}


//Update : Bloc3 terminé mais leak de mémoire (à voir plus tard).