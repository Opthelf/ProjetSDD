#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"


//Bloc 1 :

/*List * initList();
Cell * buildCell(char * ch);
void insertFirst(List * L, Cell * C);
char* ctos(Cell * c);
char * ltos(List * L);
Cell * listGet(List * L, int i);
Cell * searchList(List * L, char * str);
List * stol(char* s);
void ltof(List * L,char * path);
List * ftol(char * path);*/



/*Bloc 2 :
List * listdir(char * root_dir);
int file_exists(char *file);
void cp(char * to, char * from);
char * hashToPath(char* hash);
void blobFile(char * file);
*/

/*Bloc 3 :

*/

int main(int argc, char ** argv){

	//Bloc 1 :
	List * L1 = initList();
	Cell * C1 = buildCell("Chaine1");
	Cell * C2 = buildCell("Chaine2");
	Cell * C3 = buildCell("Chaine3");
	insertFirst(L1,C1);
	insertFirst(L1,C2);
	insertFirst(L1,C3);
	char * recup = ctos(C1);
	//char * recupT = ltos(L1);
	Cell * C4 = listGet(L1,0);
	Cell * C5 = searchList(L1,"Chaine1");
	List * L2 = stol("Je|suis|heureux");
	//printf("%s\n",ltos(stol(ltos(L2)))); //Utiliser deux fois sinon à l'envers
	//ltof(L1,"TestLtof.txt");
	//List * L3 = ftol("TestLtof.txt");
	
	//Bloc 2 :
	char cwd[50];
	List * L4 = listdir(".");
	//printf("%s\n",ltos(stol(ltos(L4))));
	//printf("%s\n",hashToPath(sha256file("main.c")));
	cp("fichier2.txt","fichier1.txt");
	blobFile("exo1.c");

	//Bloc 3 :

	WorkFile * WF1 = createWorkFile("exo1.c"); 
	WF1->hash = sha256file(WF1->name); //Sur mac ne marche pas avec un fichier txt
	WF1->mode = 777;
	
	WorkFile * WF2 = createWorkFile("exo2.c"); 
	WF2->hash = sha256file(WF2->name); //Sur mac ne marche pas avec un fichier txt
	WF2->mode = 777;
	char * chaineWorkFile = wfts(WF1);
	WF1 = stwf(chaineWorkFile);

	WorkTree * WT1 = initWorkTree();
	WT1->tab[0] = *WF1;
	WT1->n ++;
	WT1->tab[1] = *WF2;
	WT1->n ++;
	//printf("%d\n",inWorkTree(WT1,"exo2.c"));

	appendWorkTree(WT1,"main.c",sha256file("main.c"),777);
	//printf("%s\n",WT1->tab[WT1->n-1].name);
	//printf("%d\n",WT1->n);
	char * chaineWorkTree = wtts(WT1);
	printf("%s\n",chaineWorkTree);
	
	return 0;
}

//Update : Bloc 3 work in progress (appendWorkTree)