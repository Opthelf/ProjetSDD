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
	kvp * key = createKeyVal("key","value");
	char * ch = kvts(key);
	//printf("%s\n",ch);
	kvp * key2 = stkv(ch);
	char * ch2 = kvts(key2);
	//printf("%s\n",ch2);
	freeKeyVal(key2);
	freeKeyVal(key);
	free(ch);
	free(ch2);
	Commit * c = initCommit();
	
	commitSet(c,"key1","value1");
	commitSet(c,"key2","value2");
	
	Commit * c2 = createCommit("hash");
	char * val = commitGet(c,"key2");
	char * val2 = commitGet(c,"key1");
	printf("%s\n",val);
	printf("%s\n",val2);
	commitSet(c2,"key3","value3");
	commitSet(c2,"key4","value4");
	char * comtostr= cts(c2);
	//printf("%s\n",comtostr);
	Commit * c3 = stc(comtostr);
	char * ctsc3 = cts(c3);
	printf("%s\n",ctsc3);

	free(comtostr);
	freeCommit(c2);
	freeCommit(c);
	free(ctsc3);
	freeCommit(c3);

	return 0;
}


//Update Simon : 0 leak jusqu'à stwf, reste pas testé + pb freeworkTree
//	reprise à exo6.c tout marche jusqu'à stc ;

//Update Nino : J'ai mis en commentaire un free dans hashToPath parce que ça enlève quelque
//chose dont on a besoin -> NE JAMAIS FREE LES ARGUMENTS D'UNE FONCTION IL EXISTENT EN-DEHORS !!