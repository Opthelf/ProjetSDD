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

//Affiche la liste des commandes réalisables avec myGitCommit
void affiche_commande(){
    printf("Pour initialiser un répertoire myGit -> ./myGit init\n\n");
    printf("Pour afficher les différentes branches -> ./myGit list-refs\n\n");
    printf("Pour créer/actualiser une branche avec un hash d'un commit -> ./myGit create-ref <hash>\n\n");
    printf("Pour effacer une branche -> ./myGit delete-ref <branch_name>\n\n");
    printf("Pour ajouter un fichier à commit prochainement -> ./myGit add <fichier1> [option<fichier2>...]\n\n");
    printf("Pour afficher la liste des fichiers déjà add -> ./myGit list-add\n\n");
    printf("Pour retirer les fichier qui sont à commit prochainement -> ./myGit clear-add\n\n");
    printf("Pour commit dans une branche les fichiers ajoutés avec add -> ./myGit commit [-m <message>]\n\n");
    printf("Pour afficher la branche courante -> ./myGit get-current-branch\n\n");
    printf("Pour créer une branche -> ./myGit branch <branch_name>\n\n");
    printf("Pour afficher les commit d'une branche -> ./myGit print-branch <branch_name>\n\n");
    printf("Pour restaurer les fichiers depuis une branche -> ./myGit checkout-branch <branch_name>\n\n");
    printf("Pour restaurer les fichiers depuis un commit -> ./myGit checkout-commit <pattern_hash_commit>\n\n");
}

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

    //On parcourt le premier WorkTree pour rajouter les fichiers qui ne sont pas dans l'autre WorkTree
    for(int k = 0 ; k < wt1->n ; k++){

        //Si le fichier n'est pas dans le deuxième WorkTree
        if (inWorkTree(wt2,wt1->tab[k].name) == -1){
            appendWorkTree(newWT,wt1->tab[k].name,wt1->tab[k].hash,wt1->tab[k].mode);
        }
    }

    //On parcourt le deuxième WorkTree pour rajouter les fichiers qui ne sont pas dans l'autre WorkTree
    for(int k = 0 ; k < wt2->n ; k++){

        //Si le fichier n'est pas dans le premier WorkTree
        if (inWorkTree(wt1,wt2->tab[k].name) == -1){
            appendWorkTree(newWT,wt2->tab[k].name,wt2->tab[k].hash,wt2->tab[k].mode);
        }
    }
    
    return newWT;
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

    //Si la branche courante est un commit
    if (strlen(current) >= 4){
        char buff[5] = {current[0],current[1],current[2],current[3],'\0'};

        if (strcmp(buff,"HEAD") == 0){
            printf("Vous ne pouvez pas merge car vous êtes actuellement sur un commit (yet), essayez de checkout sur une branche -> merge\n");
            free(current);
            exit(EXIT_FAILURE);
        }
    }

    //Si la branche à merge est un commit
    if (strlen(remote_branch) >= 4){
        char buff[5] = {remote_branch[0],remote_branch[1],remote_branch[2],remote_branch[3],'\0'};

        if (strcmp(buff,"HEAD") == 0){
            printf("Vous ne pouvez pas merge avec un commit (yet), essayez de checkout sur une branche -> merge\n");
            exit(EXIT_FAILURE);
        }
    }

    //On récupère les WorkTree associés au dernier commit de la branche courante et remote
    WorkTree * WT_current = branchToWorkTree(current);
    WorkTree * WT_remote = branchToWorkTree(remote_branch);
    
    List * conflicts = initList();

    //On fusionne les WorkTree
    WorkTree * merged_WT = mergeWorkTrees(WT_current,WT_remote,&conflicts);

    //On libère la mémoire des WorkTree non-nécessaires à la suite
    freeWorkTree(WT_current);
    freeWorkTree(WT_remote);

    //Si il y a eu des conflits
    if (*conflicts != NULL){
        
        //On free la mémoire
        free(current);

        //Si le merged_WT n'est pas NULL on le free également
        if (merged_WT != NULL){
            freeWorkTree(merged_WT);
        }

        return conflicts;
    }

    //On récupère le hash du WorkTree
    char * hash_WT = blobWorkTree(merged_WT);

    Commit * newCommit = createCommit(hash_WT);

    //On rajoute au commit deux précédents, un pour chaque branche fusionné
    char * hash_commit_current = getRef(current);
    commitSet(newCommit,"predecessor_current",hash_commit_current);

    char * hash_commit_old = getRef(remote_branch);
    commitSet(newCommit,"predecessor_old",hash_commit_old);

    //On libère la mémoire allouée
    free(hash_WT);
    free(hash_commit_current);
    free(hash_commit_old);

    //Si le message est NULL
    if (message == NULL){
        printf("Le message est NULL -> merge");
    }

    else{
        commitSet(newCommit,"message",message);
    }

    //On crée un instantané du Commit
    char * hash_commit = blobCommit(newCommit);

    freeCommit(newCommit);

    //On met à jour les branches
    createUpdateRef("HEAD",hash_commit);
    createUpdateRef(current,hash_commit);

    //On efface la branche et on restaure le WorkTree
    deleteRef(remote_branch);
    restoreWorkTree(merged_WT,".");

    //On libère la mémoire allouée
    free(hash_commit);
    FreeList(conflicts);
    free(current);
    freeWorkTree(merged_WT);

    return NULL;
}

//La fonction crée des commits de suppresion des conflits
void createDeletionCommit(char * branch, List * conflicts, char * message){

    //On récupère la branche courante
    char * current = getCurrentBranch();

    //Si la branche courante est un commit
    if (strlen(current) >= 4){
        char buff[5] = {current[0],current[1],current[2],current[3],'\0'};

        if (strcmp(buff,"HEAD") == 0){
            printf("Vous ne pouvez pas merge car vous êtes actuellement sur un commit (yet), essayez de checkout sur une branche -> createDeletionCommit\n");
            free(current);
            exit(EXIT_FAILURE);
        }
    }

    //Si la branche à merge est un commit
    if (strlen(branch) >= 4){
        char buff[5] = {branch[0],branch[1],branch[2],branch[3],'\0'};

        if (strcmp(buff,"HEAD") == 0){
            printf("Vous ne pouvez pas merge avec un commit (yet), essayez de checkout sur une branche -> createDeletionCommit\n");
            exit(EXIT_FAILURE);
        }
    }

    //On passe sur la branche en paramètre
    if(file_exists(branch)==1){
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
                myGitAdd(WT_C->tab[i].name);
            }
        }
        //On crée le commit de suppresion 
        myGitCommit(branch,message);

        //On revient sur la branche de départ
        myGitCheckoutBranch(current);
        
        //On oublie pas de libérer la mémoire allouée :)
        free(hash_commit);
        free(path_commit);
        freeCommit(C);
        freeWorkTree(WT_C);
    }
    free(current);
}