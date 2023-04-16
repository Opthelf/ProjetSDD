#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "List.h"
#include "WorkTree.h"
#include "branch.h"
#include "Commit.h"
#include "Gestion_file.h"
#include "hash.h"
#include "merge.h"


//Initialise une cellule et lui attribue la chaine passée en paramètre
Cell * buildCell(char * ch){

    //On alloue l'espace nécessaire
	Cell * cell = (Cell*)malloc(sizeof(Cell));

    //Puis on initialise les champs de la structure
	cell->data = strdup(ch);
	cell->next = NULL;

	return cell;
}

//Libère de la mémoire la cellule passé en paramètre
void freeCell(Cell * C){ 

    //On libère le champ puis la structure
	free(C->data);
	free(C);
}

//Initialise une liste
List * initList(){

    //On alloue l'espace nécessaire
	List * L = malloc(sizeof(List));

    //On initialise à NULL la cellule
	* L = NULL;

	return L;
}

//Libère l'espace mémoire de la liste passé en paramètre
void FreeList(List * L){

    //Si la première cellule est NULL, on le mentionne, puis on libère la mémoire de la structure
	if (*L == NULL){
		printf("Liste NULL -> FreeList\n");
		free(L);
		return;
	}

    //On parcourt la liste pour la libérer 
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

    //Si la première cellule est NULL
	if (*L == NULL){
		return 0;
	}

	Cell * temp = *L;
	int i = 0;

    //On parcourt la liste pour trouver sa taille
	while(temp != NULL){
		i++;
		temp = temp->next;
	}

	return i;
}

//Insère une cellule dans une liste en première position
void insertFirst(List * L, Cell * C){
	C->next = *L;
	*L = C;
}

//Inère une cellule dans une liste en dernière position
void insertLast(List * L, Cell * C){

    //Si la premièe cellule est NULL, on lui attribue la cellule en paramètre
	if (*L == NULL){
		*L = C;
		return;
	}

    //Sinon parcourt la liste pour arriver à la fin et ajouté la cellule en paramètre
	Cell * temp = *L;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = C;
}

//Renvoie la chaine de caractère associé à une cellule
char * ctos(Cell * c){
	return c->data;
}

//Convertit une liste en chaine de caractère et la renvoie
char * ltos(List * L){

    //Si la première cellule est NULL
	if (*L == NULL){
		printf("Liste NULL -> ltos\n");

        //On renvoie la chaine de caractère vide
		return "";
	}

    //On alloue la chaine qui va stocker la liste
	char * res = malloc(sizeof(char)*1000);
	Cell * temp = *L;

    //On lui rajoute la première chaine de caractère
	strcpy(res,ctos(temp));

    //Si jamais la suivante n'est pas NULL, on lui rajoute un carctère séparateur
	if (temp->next != NULL){
		strcat(res,"|");
	}

	temp = temp->next;

    //On parcourt la liste en rajoutant chacune des chaines de caractères, et un caractère séparateur si la suivante n'est pas NULL
	while( temp != NULL ){

        //On rajoute la chaine de caractère de la cellule qu'on regarde
		strcat(res,ctos(temp));
		temp = temp->next;

        //Si la suivante n'est pas NULL, on met un carctère séparateur
		if (temp != NULL){
			strcat(res,"|");
		}
		strcat(res,"\0");
	}
	return res;
}

//Récupère la cellule à la position i dans la liste L
Cell* listGet(List* L, int i){

    //Si la première cellule est NULL, on le mentionne et on retourne NULL
	if(*L == NULL){
		printf("Liste NULL -> listGet\n");
        return NULL;
	}

    //On prépare notre variable itérative
	Cell * temp = *L;
	int k = 0;

    //On parcourt la liste
	while(temp != NULL){

        //Si on est à la bonne position, on retourne la cellule associé
		if(k == i){
			return temp;
		}

        //On met à jour les paramètres itératifs
		k++;
		temp = temp->next;
	}

    //Si la position n'a pas été trouvé, c'est que l'indice était trop grand
	printf("Indice trop grand -> listGet\n");
	return temp;
}

//Récupère la cellule dont la data associé est str dans la liste L
Cell * searchList(List * L, char * str){

    //Si la première cellule est NULL on renvoie NULL (sans rien afficher pour ne pas alourdir les messages car ça arrive plus tard)
	if(*L == NULL){
        return NULL;
	}

    //Si la chaine de caractère est NULL
    if (str == NULL){
        return NULL;
    }

    //On parcourt la liste
	Cell * temp = *L;
	while(temp != NULL){

        //Si la chaine de la cellule qu'on regarde est la même que celle recherché, on retourne la cellule
		if (strcmp(temp->data,str) == 0){
			return temp;
		}
		temp = temp->next;
	}

    //Si nous ne l'avons pas trouvé, on retourne NULL
    printf("La chaine n'est pas présente(searchList)\n");
	return NULL;
}

//Convertit une chaine de caractère en une liste
List * stol(char * s){

    //On initialise les variables qui vont nous intéresser ici
	int pos = 0;
	int n_pos = 0;
	int size = strlen(s);
	char sep = '|';
	char * ptr;
	char * result = malloc(sizeof(char)*1000);
	List * L = initList();

    //On parcourt la chaine de caractères
	while(pos < strlen(s)){
		ptr = strchr(s+pos,sep);

		if (ptr == NULL){
			n_pos = strlen(s)+1;
		}

		else{
			n_pos = ptr - s + 1;
		}

        //On copie la chaine dans ce qu'on va retourner
		memcpy(result,s+pos,n_pos-pos-1);
		result[n_pos-pos-1] = '\0';
		pos = n_pos;

        //On l'insère en dernière position dans la liste
		insertLast(L, buildCell(result));
	}
    
    //On libère la mémoire alloué
	free(result);
	return L;
}

//Convertit la liste en chaine de cractère, puis la met dans le fichier path
void ltof(List * L, char * path){

    //Si la première cellule est NULL
	if(*L == NULL){
		printf("Liste NULL(ltof)\n");
		return;
	}

    //On ouvre le fichier en mode écriture
	FILE * f = fopen(path,"w");

    //Si il y a un problème d'ouverture du fichier
	if (f == NULL){
		printf("Erreur d'ouverture du fichier %s -> ltof\n",path);

        //On exit du programme car il y a eu un problème qui n'est pas censé arriver
		exit(EXIT_FAILURE);
	}

    //On récupère la chaine de la liste
	char * chaine = ltos(L);

    //On l'écrit dans le fichier
	fprintf(f,"%s",chaine);

    //On ferme le fichier
	fclose(f);

	free(chaine);
}

//Récupère une chaine de caractère d'un fichier puis la convertit en liste
List * ftol(char * path){

    //Teste si le fichier existe
	if (file_exists(path) == 0){
		printf("Le fichier %s n'existe pas -> ftol\n",path);
		exit(EXIT_FAILURE);
	}

    //On ouvre le fichier en mode lecture
	FILE * f = fopen(path,"r");

    //Teste si il y a eu un problème d'ouverture
	if (f == NULL){
		printf("Erreur d'ouverture du fichier %s -> ftol\n",path);
		exit(EXIT_FAILURE);
	}

    //On récupère le contenu du fichier
	char buff[1000];
	fgets(buff,1000,f);

    //On ferme le fichier
	fclose(f);

    //On retourne la liste associé à la chaine de caractères
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
}

//Fusionne la deuxième liste dans la première
void fusionList(List ** first, List * second){
	insertLast(*first,*second);
}

//Retourne une sous-liste de L avec que des Cell dont les data commencent par le même pattern que pattern
List * filterList(List * L, char * pattern){

    //Si L est NULL
    if (*L == NULL){
        printf("La liste est NULL -> filterList\n");
        exit(EXIT_FAILURE);
    }

    //Si pattern est NULL
    if (pattern == NULL){
        printf("Le pattern est NULL, on suppose donc que toute la liste est retourné -> filterList\n");
        return L;
    }

    int longueur = strlen(pattern);

    if (longueur == 0){
        printf("Le pattern est la chaine vide -> filterList\n");
        return L;
    }

    List * newL = initList();
    char debut_data[longueur + 1];

    Cell * parcours = *L;

    //On parcourt la liste
    while( parcours != NULL){

        //Si la taille de la data est plus petite que le pattern
        if (strlen(parcours->data) < longueur){
            parcours = parcours->next;
            continue;
        }

        //On récupère les longueurs premiers caractères de data
        int i = 0;
        while(i<longueur){
            debut_data[i] = parcours->data[i];
            i++;
        }
        debut_data[i] = '\0';

        //Si ces caractères sont les mêmes que pattern
        if (strcmp(debut_data,pattern) == 0){
            insertFirst(newL,buildCell(parcours->data));
        }

        parcours = parcours->next;
    }
    return newL;
}