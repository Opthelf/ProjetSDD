#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "exo2.h"
#include "exo1.h"

List * initList(){
	List * L = malloc(sizeof(List));
	* L = NULL;
	return L;
}
void FreeList(List * L){
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

Cell * buildCell(char * ch){
	Cell * cell = (Cell*)malloc(sizeof(Cell));
	cell->data = strdup(ch);
	cell->next = NULL;
	return cell;
}

void freeCell(Cell * C){
	free(C->data);
	free(C);
}

void insertFirst(List * L, Cell * C){
	C->next = *L;
	*L = C;
}

void insertLast(List * L, Cell * C){
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

char * ctos(Cell * c){
	return c->data;
}

char * ltos(List * L){
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

Cell* listGet(List* L, int i){
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

Cell * searchList(List * L, char * str){
	if(*L == NULL){
		printf("Liste NULL(searchList)\n");
	}
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


List * stol(char * s){
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
	return L;
}

void ltof(List * L, char * path){
	if(*L == NULL){
		printf("Liste NULL(ltof)\n");
		return;
	}
	FILE * f = fopen(path,"w");
	char * chaine = ltos(L);
	fprintf(f,"%s",chaine);
	fclose(f);
}

List * ftol(char * path){
	FILE * f = fopen(path,"r");
	char buff[1000];
	fgets(buff,1000,f);
	return stol(buff);
}