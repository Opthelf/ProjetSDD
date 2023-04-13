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
#include "exo9.h"
#include "exo10.h"

//La fonction retourne un WorkTree des fichiers/dossier sans conflict et change une liste en y mettant les noms des fichier/dossier en conflict
WorkTree * mergeWorkTrees(WorkTree * wt1, WorkTree * wt2, List ** conflicts){
    
    //Si le premier WorkTree est NULL
    if (wt1 == NULL || wt1->n == 0){
        
        //Si le deuxième WorkTree est NULL
        if (wt2 == NULL || wt2->n == 0){
            printf("Les deux WorkTree sont nuls ou vide -> mergeWorkTree\n");
            return NULL;
        }

        return wt2;
    } 

    //Si le deuxième WorkTree est NULL
    if (wt2 == NULL || wt2->n == 0){
        
        return wt1;
    }

    List * temp_conflicts = *conflicts; 
    WorkTree * newWT = initWorkTree();

    int i = 0;
    int j = 0;

    //Parcours du premier WorkTree
    while(i < wt1->n){

        WorkFile * WF1 = &(wt1->tab[i]);

        //Parcours du deuxième WorkTree
        while (j < wt2->n){

            WorkFile * WF2 = &(wt2->tab[j]);

            //Si les WorkFile ont les mêmes noms
            if (strcmp(WF1->name,WF2->name) == 0){

                //Si les WorkFile ont les mêmes hash
                if (strcmp(WF1->hash,WF2->hash) == 0){
                    appendWorkTree(newWT,WF1->name,WF1->hash,WF1->mode);
                }

                //Si les WorkFile n'ont pas les mêmes hash on l'ajoute à la liste des conflicts
                else{
                    insertFirst(temp_conflicts,buildCell(WF1->name));
                }
            }
            j++;
        }

        i++;
        j = 0;
    }

    return newWT;
}