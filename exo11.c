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


//Retourne le WorkTree associé au dernier commit d'une branche
WorkTree * branchToWorkTree(char * branch_name){

    //Si la chaine de caractère est NULL
    if (branch_name == NULL){
        printf("Le nom de la branche à merge est NULL -> merge\n");
        exit(EXIT_FAILURE);
    }

    //Si la branche n'existe pas
    if (branchExists(branch_name) == 0){
        printf("La branche %s n'existe pas -> merge\n",branch_name);
        exit(EXIT_FAILURE);
    }

    //On récupère le hash du dernier commit de la branche
    char * hash = getRef(branch_name);

    //On récupère le path vers le commit, puis le commit lui-même
    char * path = hashToPathCommit(hash);
    Commit * C = ftc(path);

    //On récupère le hash du WorkTree dans le commit
    char * hashWT = commitGet(C,"tree");

    //On récupère le path vers ce WorkTree, puis le WorkTree lui-même
    char * pathWT = hashToPathWorkTree(hashWT);
    WorkTree * WT = ftwt(pathWT);

    //On libère la mémoire alloué
    free(hash);
    free(path);
    freeCommit(C);
    free(hashWT);
    free(pathWT);

    return WT;
}

//La fonction merge la branche courante avec la branche en paramètre si il n'y a aucun conflict
List * merge(char * remote_branch, char * message){

    //Si la chaine de caractère est NULL
    if (remote_branch == NULL){
        printf("Le nom de la branche à merge est NULL -> merge\n");
        exit(EXIT_FAILURE);
    }

    //Si la branche n'existe pas
    if (branchExists(remote_branch) == 0){
        printf("La branche %s n'existe pas -> merge\n",remote_branch);
        exit(EXIT_FAILURE);
    }

    //On récupère la branche courante
    char * current = getCurrentBranch();

    //On récupère les WorkTree associés au dernier commit de la branche courante et remote
    WorkTree * WT_current = branchToWorkTree(current);
    WorkTree * WT_remote = branchToWorkTree(remote_branch);
    
    List * conflicts = initList();

    WorkTree * merged_WT = mergeWorkTrees(WT_current,WT_remote,&conflicts);

    freeWorkTree(WT_current);
    freeWorkTree(WT_remote);

    //Si il y a eu des conflits
    if (conflicts != NULL){
        
        //On free la mémoire
        free(current);

        //Si le merged_WT n'est pas NULL on le free également
        if (merged_WT != NULL){
            freeWorkTree(merged_WT);
        }

        return conflicts;
    }

    char * hash_WT = blobWorkTree(merged_WT);

    Commit * newCommit = createCommit(hash_WT);

    char * hash_commit_current = getRef(current);
    commitSet(newCommit,"predecessor_current",hash_commit_current);

    char * hash_commit_old = getRef(remote_branch);
    commitSet(newCommit,"predecessor_old",hash_commit_old);

    free(hash_commit_current);
    free(hash_commit_old);

    if (message == NULL){
        printf("Le message est NULL -> merge");
    }

    else{
        commitSet(newCommit,"message",message);
    }

    char * hash_commit = blobCommit(newCommit);

    freeCommit(newCommit);

    createUpdateRef("HEAD",hash_commit);
    createUpdateRef(current,hash_commit);

    free(hash_commit);

    deleteRef(remote_branch);

    restoreWorkTree(merged_WT,".");

    freeWorkTree(merged_WT);

    return NULL;
}


//La fonction crée des commits de suppresion des conflits
void createDeletionCommit(char * branch, List * conflicts, char * message){

    //On récupère la branche courante
    char * current = getCurrentBranch();

    //On passe sur la branche en paramètre
    myGitCheckoutBranch(branch);

    
    //On récupère le path vers le commit, puis le commit lui-même
    char * hash_commit = getRef(branch);
    char * path_commit = hashToPathCommit(hash_commit);
    Commit * C = ftc(path_commit);

    //On récupère le WorkTree associé au commit
    WorkTree * WT_C = branchToWorkTree(branch);

    //On clean add
    system("rm .add");

    //On parcours le WorkTree pour ajouter les fichiers sans conflits à notre branche
    for(int i = 0 ; i < WT_C->n ; i++){

        //On teste si le fichier/répertoire que l'on regarde n'est pas dans la liste des conflits
        Cell * C = searchList(conflicts,WT_C->tab[i].name);
        if (C == NULL){

            //On ajoute les fichiers sans conflits dans .add
            myGitAdd(C->data);
        }
    }

    //On crée le commit de suppresion 
    myGitCommit(branch,message);

    //On revient sur la branche de départ
    myGitCheckoutBranch(current);

    //On oublie pas de libérer la mémoire allouée :)
    free(current);
    free(hash_commit);
    free(path_commit);
    freeCommit(C);
    freeWorkTree(WT_C);
}

