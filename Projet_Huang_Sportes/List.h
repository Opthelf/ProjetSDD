#ifndef LIST_H
#define LIST_H

//Structure Cell
typedef struct cell{
	char * data;
	struct cell * next;
}Cell;

//Structure List
typedef Cell * List;

Cell * buildCell(char * ch);
void freeCell(Cell * C);
List * initList();
void FreeList(List * L);
int tailleList(List * L);
void insertFirst(List * L, Cell * C);
void insertLast(List * L, Cell * C);
char * ctos(Cell * c);
char * ltos(List * L);
Cell* listGet(List* L, int i);
Cell * searchList(List * L, char * str);
List * stol(char * s);
void ltof(List * L, char * path);
List * ftol(char * path);
void tri_par_choix(List * conflicts, List ** L_current, List ** L_branch);
void fusionList(List ** first, List * second);
List * filterList(List * L, char * pattern);

#endif
