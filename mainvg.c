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

int main(){


WorkFile * WF1 = createWorkFile("Fichier1");
freeWorkFile(WF1);
WorkTree * WT1 = initWorkTree();
freeWorkTree(WT1);
int d = inWorkTree(WT1,"Fichier1");
int d2 = inWorkTree(WT1,"Fichier2");
return 0;
}