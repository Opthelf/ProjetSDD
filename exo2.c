#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "exo2.h"
#include "exo1.h"

List * initList(){ //Initialise une liste
	List * L = malloc(sizeof(List));
	* L = NULL;
	return L;
}
void FreeList(List * L){ //Libère l'espace mémoire de la liste passé en paramètre
	if (*L == NULL){
		printf("Liste NULL(FreeList)\n");
		free(L);
		return;
	}
	Cell * temp = *L;
	while(temp != NULL){
		Cell * temp2 = temp;
		temp = temp->next;
		freeCell(temp2);
	}
	free(L);
}

//Renvoie la taille de la liste
int tailleList(List * L){
	if (*L == NULL){
		return 0;
	}

	Cell * temp = *L;
	int i = 0;

	while(temp != NULL){
		i++;
		temp = temp->next;
	}

	return i;
}

Cell * buildCell(char * ch){ //Initialise une cellule et lui attribue la chaine passée en paramètre
	Cell * cell = (Cell*)malloc(sizeof(Cell));
	cell->data = strdup(ch);
	cell->next = NULL;
	return cell;
}

void freeCell(Cell * C){ //Libère de la mémoire la cellule passé en paramètre
	free(C->data);
	free(C);
}

void insertFirst(List * L, Cell * C){ //Insère une cellule dans une liste en première position
	C->next = *L;
	*L = C;
}

void insertLast(List * L, Cell * C){ //Inère une cellule dans une liste en dernière position
	if (*L == NULL){
		*L = C;
		return;
	}
	Cell * temp = *L;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = C;
}

char * ctos(Cell * c){ //Renvoie la chaine de caractère associé à une cellule
	return c->data;
}

char * ltos(List * L){ //Convertit une liste en chaine de caractère et la renvoie
	if (*L == NULL){
		printf("Liste NULL(ltos)\n");
		return "";
	}
	char * res = malloc(sizeof(char)*1000);
	Cell * temp = *L;
	strcpy(res,ctos(temp));
	if (temp->next != NULL){
			strcat(res,"|");
		}
	temp = temp->next;
	while( temp != NULL ){
		strcat(res,ctos(temp));
		temp = temp->next;
		if (temp != NULL){
			strcat(res,"|");
		}
		strcat(res,"\0");
	}
	return res;
}

Cell* listGet(List* L, int i){ //Récupère la cellule à la position i dans la liste L
	if(*L == NULL){
		printf("Liste NULL(listGet)\n");
	}	
	Cell * temp = *L;
	int k = 0;
	while(temp != NULL){
		if(k == i){
			return temp;
		}
		k++;
		temp = temp->next;
	}  
	printf("Indice trop grand(listget)\n");
	return temp;
}

Cell * searchList(List * L, char * str){ //Récupère la cellule dont la data associé est str dans la liste L
	/*if(*L == NULL){
		printf("Liste NULL(searchList)\n");
	}*/
	Cell * temp = *L;
	while(temp != NULL){
		if (strcmp(temp->data,str) == 0){
			return temp;
		}
		temp = temp->next;
	}
	printf("La chaine n'est pas présente(searchList)\n");
	return NULL;
}


List * stol(char * s){ //Convertit une chaine de caractère en une liste
	int pos = 0;
	int n_pos = 0;
	int size = strlen(s);
	char sep = '|';
	char * ptr;
	char * result = malloc(sizeof(char)*1000);
	List * L = initList();
	while(pos < strlen(s)){
		ptr = strchr(s+pos,sep);
		if (ptr == NULL){
			n_pos = strlen(s)+1;
		}
		else{
			n_pos = ptr - s + 1;
		}
		memcpy(result,s+pos,n_pos-pos-1);
		result[n_pos-pos-1] = '\0';
		pos = n_pos;
		insertLast(L, buildCell(result));
	}
	free(result);
	return L;
}

void ltof(List * L, char * path){ //Convertit la liste en chaine de cractère, puis la met dans le fichier path
	if(*L == NULL){
		printf("Liste NULL(ltof)\n");
		return;
	}
	FILE * f = fopen(path,"w");

	if (f == NULL){ //Si il y a un problème d'ouverture du fichier
		printf("Erreur d'ouverture du fichier %s -> ltof\n",path);
		exit(EXIT_FAILURE);
	}
	char * chaine = ltos(L);
	fprintf(f,"%s",chaine);
	fclose(f);
	free(chaine);
}

List * ftol(char * path){ //Récupère une chaine de caractère d'un fichier puis la convertit en liste
	if (file_exists(path) == 0){ //Teste si le fichier existe
		printf("Le fichier %s n'existe pas -> ftol\n",path);
		exit(EXIT_FAILURE);
	}
	FILE * f = fopen(path,"r");

	if (f == NULL){ //Teste si il y a eu un problème d'ouverture
		printf("Erreur d'ouverture du fichier %s -> ftol\n",path);
		exit(EXIT_FAILURE);
	}

	char buff[1000];
	fgets(buff,1000,f);
	fclose(f);
	return stol(buff);
}

//Modifie les deux listes L_current et L_branch selon les choix de l'utilisateur sur les fichiers dans conflicts
void tri_par_choix(List * conflicts, List ** L_current, List ** L_branch){
	int choix;

	//On parcourt la liste des conflits
	while (*conflicts != NULL){

		//On récupère le choix de l'utilisateur de la version à garder
		printf("Quel est votre choix pour %s ?\n",(*conflicts)->data);
		scanf("%d",&choix);

		//Si le choix ne correspond à aucune des options
		if (choix != 1 || choix != 2){
			printf("%d n'est pas une option ! Résolution des conflits annulés\n",choix);
			exit(EXIT_FAILURE);
		}

		//Si il choisit 1, qui correspond à la branche courante
		if (choix == 1){
			insertLast(*L_current,*conflicts);
		}

		//Si il choisit 2, qui correspond à la branche à merge
		if (choix == 2){
			insertLast(*L_branch,*conflicts);
		}

		*conflicts = (*conflicts)->next;
	}
	printf("maissss\n");
	if (L_current == NULL){
		printf("voici le pb\n");
	}
	char * chaine = ltos(*L_current);
	printf("%s\n",chaine);
}

//Fusionne la deuxième liste dans la première
void fusionList(List ** first, List * second){
	insertLast(*first,*second);
}