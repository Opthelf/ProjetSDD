#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"
#include "exo8.h"

//Initialise un fichier qui donne le nom de la branche courante
void initBranch(){

    //Si le fichier n'existe pas déjà
    if (!file_exists(".current_branch")){
        system("echo master > .current_branch");
    }
}

//Vérifie qu'une branche existe
int branchExists(char* branch){

    //Si le fichier .refs n'existe pas
    if (file_exists(".refs") == 0){
        printf("Dossier .refs non initialisé, aucune branches existantes -> branchExists\n");
        exit(EXIT_FAILURE);
    }

    char buff[100];
    strcpy(buff,"");
    strcat(buff,".refs/");
    strcat(buff,branch);

    //Si la branche n'existe pas
    if (file_exists(buff) == 0){
        return 0;
    }

    return 1;
}

//Crée une branche avec le dernier commit
void createBranch(char* branch){

    //On récupère le dernier commit dans HEAD et on le copie dans la nouvelle branche
    char * chaine = getRef("HEAD");

    if (chaine == NULL){
        printf("On ne peut pas créer de branches si il n'y a eu aucun commit au départ !\n");
        exit(EXIT_FAILURE);
    }

    createUpdateRef(branch,chaine);
    free(chaine);
}

//Renvoie la branche courante
char* getCurrentBranch(){

    //Si le fichier .current_branch n'existe pas
    if (file_exists(".current_branch") == 0){
        printf("Fichier .current_branch non-initialisé -> getCurrentBranch\n");
        exit(EXIT_FAILURE);
    }

    FILE * f = fopen(".current_branch","r");

    //Si il y a eu un problème d'ouverture
    if (f == NULL){
        printf("Erreur lors de l'ouverture du fichier -> getCurrentBranch\n");
        return NULL;
    }

    char * buff = malloc(sizeof(char)*300);
    strcpy(buff,"");

    //On récupère le nom de la branche dans le fichier
    if (fgets(buff,300,f) == NULL){
        printf("Le fichier .current_branch est vide -> getCurrentBranch\n");
        return NULL;
    }
    
    if( buff[strlen(buff)-1] == '\n' ){
        buff[strlen(buff)-1] = 0;
    }

    fclose(f);
    return buff;
}

//Transforme le hash d'un commit en path vers son instantané
char * hashToPathCommit(char * hash){

    //On transforme le hash en path
    char * buff = hashToPath(hash);
    buff = realloc(buff, sizeof(char) * (strlen(buff)+3) );

    //On ajoute le .c des instantanés de commit
    strcat(buff,".c");
    strcat(buff,"\0");
    return buff;
}

//Affiche la liste des commit d'une branche
void printBranch(char * branch){

    //Si la branche n'existe pas
    if (branchExists(branch) == 0){
        printf("La branche %s n'existe pas -> printBranch\n",branch);
        exit(EXIT_FAILURE);
    }

    char * commit_hash = getRef(branch);

    //Si le hash du commit est NULL
    if (commit_hash == NULL){
        printf("Le hash du commit dans la branche %s est NULL -> printBranch\n",branch);
        exit(EXIT_FAILURE);
    }

    //Récupère le path vers ce commit, puis le commit lui-même
    char * path = hashToPathCommit(commit_hash);
    Commit * c = ftc(path);
    

    //Tant que il y a un predecessor on continue
    while (c != NULL){

        //Si le message n'est pas NULL on l'affiche avec le hash du commit
        char * message = commitGet(c,"message");
        
        if(message != NULL){ 
            printf("%s -> %s \n",commit_hash,message);
            free(message);
        }

        //Sinon on affiche simplement le hash du commit
        else{
            printf("%s \n",commit_hash);
        }
        free(commit_hash);
        commit_hash = commitGet(c,"predecessor");

        //Si le predecessor n'est pas NULL on récupère le predecessor
        if(commit_hash != NULL){
            free(path);
            path = hashToPathCommit(commit_hash);
            freeCommit(c);
            c = ftc(path);
        }

        //Sinon on le free et on initialise c à NULL pour sortir
        else{
            freeCommit(c);
            c = NULL;
        }
    }

    free(path);
    free(commit_hash);
}


//Retourne une liste de tout les commit dans la branche dont le dernier commit est passé en paramètre
List * branchList(char * commit_hash){

    List * L = initList();

    //Si il n'y a pas de hash dans la branche, donc pas de commit
    if (commit_hash == NULL){
        return NULL;
    }

    char * path = hashToPathCommit(commit_hash);
    Commit * c = ftc(path);

    char * commit_pred = commit_hash;

    //On parcourt notre liste de commit
    while(c != NULL){

        //On insère dans la liste la valeur
        insertFirst(L,buildCell(commit_pred));

        //Si le prédécessor existe on actualise la variable itérative
        commit_pred = commitGet(c,"predecessor");

        char * commit_hash_old = commitGet(c,"predecessor_old");

        if (commit_hash_old != NULL){
            List * old = branchList(commit_hash_old);
            free(commit_hash_old);
            fusionList(&L,old);
            free(old);
        }

        char * commit_hash_current = commitGet(c,"predecessor_current");

        if (commit_hash_current != NULL){
            List * current = branchList(commit_hash_current);
            free(commit_hash_current);
            fusionList(&L,current);
            free(current);
        }

        if(commit_pred != NULL){
            free(path);
            path = hashToPathCommit(commit_pred);
            freeCommit(c);
            c = ftc(path);
        }

        //Sinon on libère la structure et on l'initialise à NULL
        else{
            freeCommit(c);
            c = NULL;
        }
        
    }
    free(commit_pred);
    free(path);

    return L;
}


//Récupère le commit de toutes les branches
List * getAllCommits(){

    List * L = initList();
    List * content = listdir(".refs");
    for(Cell * ptr = *content ; ptr != NULL ; ptr = ptr->next){
        if(ptr->data[0]=='.'){
            continue;
        }
    
        char * tmp = strdup(ptr->data);

        //Si la branche n'existe pas
        if (branchExists(tmp) == 0){
            printf("La branche %s n'existe pas -> branchList\n",tmp);
        exit(EXIT_FAILURE);
        }

        char * commit_hash = getRef(tmp);

        List * list = branchList(commit_hash);

        free(tmp);
        
        if(commit_hash != NULL){
            free(commit_hash);
        }

        if (*list == NULL){
            free(list);
            continue;
        }

        Cell * cell = *list;
        while(cell != NULL){
            if(searchList(L,cell->data) == NULL){
                Cell * c = buildCell(cell->data);
                insertFirst(L,c);
            }

            cell = cell->next;
            
        }
        FreeList(list);
    }
    FreeList(content);
    return L;
}