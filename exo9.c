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
    WorkTree * WT = ftwt(path_WorkTree);

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

    char * head = getRef("HEAD");
    if (head == NULL){
        char buff[300];
        sprintf(buff,"echo %s > .current_branch",branch);
        system(buff);
        
        return;
    }

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
    
    if (hash == NULL){
        char * current = getCurrentBranch();
        char * hash_c = getRef(current);
        createUpdateRef(branch,hash_c);
        free(head);
        free(hash);
        free(current);
        free(hash_c);
        return;
    }

    createUpdateRef("HEAD",hash);

    //On restaure les fichiers comme ils étaient lors du dernier commit
    restoreCommit(hash);
    free(head);
    free(hash);
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
           // printf("Parcoursdata -> %s\n",parcours->data);
        }

        parcours = parcours->next;
    }
    return newL;
}

//Restaure les fichiers sauvegardés dans le commit dont le hash commence par pattern
void myGitCheckoutCommit(char* pattern){
    if (pattern == NULL){
        printf("Le pattern a été oublié, par quoi commence votre hash du commit ? -> myGitChechkoutCommit\n");
        exit(EXIT_FAILURE);
    }

    List * AllCommits = getAllCommits();

    //Si aucun commit n'est récupéré
    if (*AllCommits == NULL){
        printf("Aucun commit récupéré -> myGitChechkoutCommit\n");
        free(AllCommits);
        exit(EXIT_FAILURE);
    }

    char * ls = ltos(AllCommits);
    free(ls);
    List * Commit_Pattern = filterList(AllCommits,pattern);
    int longueur = tailleList(Commit_Pattern);

    //Si la liste récupéré est NULL
    if (longueur < 1){
        FreeList(AllCommits);
        FreeList(Commit_Pattern);
        printf("Votre recherche de commit n'a pas abouti, Commit_Pattern est NULL -> myGitCheckout\n");
        exit(EXIT_FAILURE);
    }
    
    //Si la taille de la liste est supérieure à 1
    if (longueur > 1){
        //Afficher toute la liste ici
        char * list_commit_pattern = ltos(Commit_Pattern);
        printf("Liste des commits correspondant au pattern : \n%s\n",list_commit_pattern);
        free(list_commit_pattern);
        FreeList(AllCommits);
        FreeList(Commit_Pattern);
        printf("Votre pattern n'est pas assez précis, plusieurs possibilités : veuillez réessayer -> myGitCheckout\n");
        exit(EXIT_FAILURE);
    }

    char * hash_commit = Commit_Pattern[0]->data;
    createUpdateRef("HEAD",hash_commit);
    restoreCommit(hash_commit);
    char * head = getRef("HEAD");

    //peut être mieux implémenté
    char buff1[100]= "HEAD detached at ";
    
    for(int i=0;i<7;i++){
        buff1[i+17]=head[i];
    }
    
    char buff2[300];
    sprintf(buff2,"echo %s > .current_branch",buff1);
    system(buff2);

    free(head);
    FreeList(AllCommits);
    FreeList(Commit_Pattern);
}