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


//La fonction restaure les fichiers sauvegardés via le commit
void restoreCommit(char * hash_commit){

    //Si le hash_commit est NULL
    if (hash_commit == NULL){
        printf("Le hash est NULL -> restoreCommit\n");
        exit(EXIT_FAILURE);
    }

    //On récupère le path vers le commit
    char * path_commit = hashToPathCommit(hash_commit);

    //On récupère le commit même
    Commit * C = ftc(path_commit);

    //On récupère le hash du WorkTree à restorer
    char * hash_WorkTree = commitGet(C,"tree");

    //Si le hash n'a pas été trouvé
    if (hash_WorkTree == NULL){
        printf("On a pas réussi à récupérer la valeur de tree dans le commit -> restoreCommit\n");
        exit(EXIT_FAILURE);
    }

    //On le transforme en path vers le WorkTree
    char * path_WorkTree = hashToPath(hash_WorkTree);
    strcat(path_WorkTree,".t");

    //On récupère le WorkTree à restorer dans le fichier
    WorkTree * WT = initWorkTree();
    WT = ftwt(path_WorkTree);

    //On le restaure
    restoreWorkTree(WT,".");

    //On free
    free(path_commit);
    freeCommit(C);
    free(hash_WorkTree);
    free(path_WorkTree);
    freeWorkTree(WT);
}

//Permet un passage de la branche courante vers la branche en paramètre
void myGitCheckoutBranch(char * branch){

    //Si la chaine branch est NULL
    if (branch == NULL){
        printf("La branche en paramètre est NULL -> myGitCheckoutBranch\n");
        exit(EXIT_FAILURE);
    }

    //Si la branche n'existe pas
    if (branchExists(branch) == 0){
        printf("La branche %s n'existe pas -> myGitCheckoutBranch\n",branch);
        exit(EXIT_FAILURE);
    }

    //On met à jour le fichier .current_branch
    char buff[300];
    sprintf(buff,"echo %s > .current_branch",branch);
    system(buff);

    //On met à jour le fichier HEAD pour pointer sur le dernier commit de la nouvelle branche courante
    char * hash = getRef(branch);
    createUpdateRef("HEAD",hash);

    //On restaure les fichiers comme ils étaient lors du dernier commit
    restoreCommit(hash);

    free(hash);

}

//Retourne une sous-liste de L avec que des Cell dont les data commencent par le même pattern que pattern
List * filterList(List * L, char * pattern){
    //ATTENTION MESSAGE POUR SIMON (moi je sais déjà)
    //-> Dans le main, il faudra libérer uniquement la liste en paramètre, pas celle retournée


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
        strncpy(debut_data,parcours->data,longueur);
        strcat(debut_data,"\0");
        printf("debut_data -> %s\n",debut_data);
        //Si ces caractères sont les mêmes que pattern
        if (strcmp(debut_data,pattern) == 0){
            printf("oui\n");
            printf("cell -> %s\n",ctos(parcours));
            insertFirst(newL,parcours);
        }


        parcours = parcours->next;
    }
    return newL;
}