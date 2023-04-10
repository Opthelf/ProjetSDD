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

int main(int argc,char * argv[]){
    if (argc < 2){
        printf("Pas assez d'argument !\n");
    }
    if (strcmp(argv[1],"init") == 0){
        initRefs();
        return 0;
    }
    if (strcmp(argv[1],"list-refs") == 0){
        if (file_exists(".refs") == 0){
            printf("Aucun répertoire\n");
        }
        else{
            printf("Refs :\n");
            List * L = listdir(".refs");
            Cell * temp = *L;
            while(temp != NULL){
                if ((strcmp(temp->data,".")) != 0 && (strcmp(temp->data,"..") != 0)){
                    printf("\t%s\n",temp->data);
                }
                temp = temp->next;
            }
            //freeCell(temp); cause une segmentation fault, je ne sais pas si il y a une leak
            FreeList(L);
        }
        return 0;
    }
    if (strcmp(argv[1],"create-ref") == 0){
        if (argc != 4){
            printf("Pas assez d'argument pour créer un ref !\n");
            return 0;
        }
        createUpdateRef(argv[2],argv[3]);
        return 0;
    }
    if (strcmp(argv[1],"delete-ref") == 0){
        if (argc != 3){
            printf("Pas assez d'argument pour delete un ref !\n");
            return 0;
        }
        deleteRef(argv[2]);
        return 0;
    }
    if (strcmp(argv[1],"add") == 0){
        int i = 2;
        while(i < argc){
            myGitAdd(argv[i]);
            i++;
        }
        return 0;
    }
    if (strcmp(argv[1],"list-add") == 0){
        if (file_exists(".add") == 0){
            printf("Aucun fichier dans la staging area\n");
        }
        else{
            WorkTree * WT = ftwt(".add");
            printf("Added :\n%s\n",wtts(WT));
            //freeWorkTree(WT); Pareil, pas sûr que ce soit nécessaire, erreur de segmentation si on essaye
        }
        return 0;
    }
    if (strcmp(argv[1],"clear-add") == 0){
        system("rm .add");
        return 0;
    }
    if (strcmp(argv[1],"commit") == 0){
        if ((argc < 3) || (argc == 4)){
            printf("Nombre d'arguments incorrects pour réaliser un commit\n");
            return 0;
        }
        if (argc == 3){
            myGitCommit(argv[2],NULL);
            return 0;
        }
        if (argc == 5){
            if (strcmp(argv[4],"-m")){
                myGitCommit(argv[2],argv[4]);
            }
        }
    }
    return 0;
}