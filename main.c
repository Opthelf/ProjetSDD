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

int main(int argc, char ** argv){
	//Tout les anciens tests sont dans main.txt bien triés pour faire de la place ici

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
	
<<<<<<< HEAD
	Commit * c2 = createCommit("hash");
	char * val = commitGet(c,"key2");
	char * val2 = commitGet(c,"key1");
	//printf("%s\n",val);
	//printf("%s\n",val2);
	commitSet(c2,"key3","value3");
	commitSet(c2,"key4","value4");
	char * comtostr= cts(c2);
	//printf("%s\n",comtostr);
	Commit * c3 = stc(comtostr);
	char * ctsc3 = cts(c3);
	//printf("%s\n",ctsc3);
	ctf(c3,"testex6.txt");
	Commit * c4 =  ftc("testex6.txt");
	char *ctsc4 = cts(c4);
	printf("%s\n",ctsc4);
	char * hashc4 = blobCommit(c4);
	freeCommit(c4);
	free(ctsc4);
	free(comtostr);
	freeCommit(c2);
	freeCommit(c);
	free(ctsc3);
	freeCommit(c3);
	free(hashc4);
=======
	//char * save= saveWorkTree(WT,".");
	
	//free(path);
	//free(blob);
	//free(hash);
	//free(htf);
	//freeWorkTree(WT);
	//free(save);
	//free(h1);
	//free(h2);
	/*int gcm = getChmod("exo5.txt");
	printf("getChmod(\"exo5.txt\") = %d\n",gcm);*/
	WorkTree * WT = initWorkTree();
	appendWorkTree(WT,"name.txt",NULL,777);
	char * chaineWT = wtts(WT);
	printf("wt :\n%s\n",chaineWT);

	freeWorkTree(WT);
	free(chaineWT);
>>>>>>> b4c853ba5e887996abcceeb5b742950578b2df07
	return 0;
}


/*
Update Simon : ex6 terminé plus de leak mais tu peux tjrs faire plus de tests ;
exo8 terminé mais rien n'est testé ,en tout cas pour les fonctions que j'ai faites (à partir de printBranch)
*/


/*
Update Nino -> restoreWorkTree fonctionne !!! demande pour tester c'est quand même particulier
-> exo8 commencé, pour l'instant OK, mais petit doute sur le fonctionnement de myGitCommit, à rechecker 

Exo 1 à 4 revu, solidifié le code avec des tests, commentaires brefs pour expliquer ce que fait la fonction et testé jusqu'au milieu de l'exo 4 à peu près
*/

