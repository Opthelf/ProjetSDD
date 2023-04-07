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
	List * L1 = initList();
	Cell * C1 = buildCell("Chaine1");
	Cell * C2 = buildCell("Chaine2");
    Cell * C3 = buildCell("Chaine3");
	char * recup = ctos(C1);
    char * recupT = ltos(L1);
	Cell * C4 = listGet(L1,0);
    Cell * C5 = searchList(L1,"Chaine1");
	insertLast(L1,C1);
	insertLast(L1,C2);
    insertLast(L1,C3);
	
	List * L2 = stol("Je|suis|heureux");
	char * ltosL2=ltos(L2);
	printf("%s\n",ltosL2);
	ltof(L1,"TestLtof.txt");
	free(ltosL2);
	FreeList(L2);
	FreeList(L1);
	List * L3 = ftol("TestLtof.txt");
	FreeList(L3);
    /*
    
    
    
    
    
    
    
    
*/
	return 0;
}


//Update : 0 leak exo2.c