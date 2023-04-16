#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "List.h"
#include "WorkTree.h"
#include "branch.h"
#include "Commit.h"
#include "Gestion_file.h"
#include "hash.h"
#include "merge.h"

//La fonction crée le répertoire .refs et les fichiers master et HEAD dedans si ce n'est pas déjà fait
void initRefs(){ 

    //Si le dossier n'existe pas déjà
    if(!file_exists(".refs")){
        system("mkdir .refs");    
        system("touch .refs/master");
        system("touch .refs/HEAD");
    }
}

void createUpdateRef(char* ref_name, char* hash){

    //Si le hash est NULL
    if (hash == NULL){
        printf("Le hash est NULL !(createUpdateRef)\n");
        char buff1[300];
        strcpy(buff1,"");

        //On crée le fichier mais il sera vide
        sprintf(buff1,"touch .refs/%s",ref_name);
        system(buff1);

        return;
    }

    //Insère le hash dans la ref
    char buff[300];
    strcpy(buff,"");
    sprintf(buff,"echo %s > .refs/%s",hash,ref_name);
    system(buff);
}

//La fonction efface une référence
void deleteRef(char* ref_name){

    //récupère la chaine de carctère du path vers le fichier
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);

    //Si le fichier n'existe pas
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(deleteRef)\n",buff);
        exit(EXIT_FAILURE);
    }

    //Supprime la référence
    sprintf(buff,"rm .refs/%s",ref_name);
    system(buff);
}

//Récupère le hash d'une référence
char* getRef(char* ref_name){

    //récupère la chaine de carctère du path vers le fichier
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",ref_name);

    //Si le fichier n'existe pas
    if (file_exists(buff) == 0){
        printf("Le fichier %s n'existe pas à l'endroit indiqué !(getRef)\n",buff);
        return NULL;
    }

    //On ouvre le fichier en lecture
    FILE * f = fopen(buff,"r");

    //Si il y a eu un problème d'ouverture
    if (f == NULL){
        printf("Erreur %d d'ouverture lors du fichier(getRef)\n",errno);
        return NULL;
    }

    //On alloue la chaine pour récupérer la référence
    char * recup = malloc(sizeof(char)*300);
    strcpy(recup,"");

    //On récupère le hash dans le fichier
    if (fgets(recup,256,f) == NULL){

        //Si on ne récupère rien, on libère la mémoire et on ferme le fichier
        free(recup);
        fclose(f);
        return NULL;
    }

    //On enlève le retour à la ligne de la chaine et on ferme le fichier
    sscanf(recup,"%s\n",recup);
    fclose(f);
    return recup;
}

//Ajoute le fichier/dossier dans le fichier .add sous forme de WorkFile/WorkTree
void myGitAdd(char* file_or_folder){
    WorkTree * wt;

    //On récupère la branche courante
    char * current = getCurrentBranch();

    //Si la chaine a au moins quatre caractère
    if (strlen(current) >= 4){

        //On récupère les 4 premiers caractères
        char buff[5] = {current[0],current[1],current[2],current[3],'\0'};

        //Si jamais ils commencent par add, on se trouve actuellement sur un commit et on ne peut pas ajouter de fichiers
        if (strcmp(buff,"HEAD") == 0){

            printf("Vous ne pouvez pas ajouter de fichiers à add car vous êtes actuellement sur un commit, essayez de checkout sur une branche -> myGitAdd\n");
            free(current);
            exit(EXIT_FAILURE);
        }
    }

    //On libère la chaine allouée
    free(current);

    //Si le fichier .add n'existe pas
    if (file_exists(".add") == 0){
        system("touch .add");
        wt = initWorkTree();
    }

    //Si il existe on récupère directement le WorkTree à l'intérieur
    else{
        wt = ftwt(".add");
    }

    //Si le fichier existe on le rajoute au WorkTree
    if (file_exists(file_or_folder) != 0){

        //On ajoute au WorkTree le fichier/dossier, et on écrit le WorkTree dans le fichier .add
        appendWorkTree(wt,file_or_folder,NULL,getChmod(file_or_folder));
        wttf(wt,".add");
    }

    //Sinon on le signale
    else{
        printf("Le fichier ou répertoire %s n'existe pas !(myGitAdd)\n",file_or_folder);
    }

    //On libère la mémoire allouée
    freeWorkTree(wt);
}

//Commit dans une branche
void myGitCommit(char* branch_name, char* message){
    
    //Si le fichier .refs n'existe pas
    if (file_exists(".refs") == 0){

        printf("Initialiser d'abord les références du projet -> (myGitCommit)\n");
        exit(EXIT_FAILURE);
    }

    //On récupère la path vers la référence
    char buff[100];
    strcpy(buff,"");
    sprintf(buff,".refs/%s",branch_name);

    //Si la référence n'existe pas
    if (file_exists(buff) == 0){
        printf("La branche %s n'existe pas -> (myGitCommit)\n",branch_name);
        exit(EXIT_FAILURE);
    }

    //Si le fichier .add n'existe pas
    if (file_exists(".add") == 0){
        printf("Aucun fichier n'a été ajouté à add, il n'existe pas, impossible de commit -> myGitCommit\n");
        exit(EXIT_FAILURE);
    }

    //On récupère la branche courante
    char * current = getCurrentBranch();
    
    //Si la branche courante n'est pas la branche sur laquelle on veut commit
    if (strcmp(current,branch_name) != 0){
        printf("Vous essayez de commit sur une branche qui n'est pas la branche courante, opération impossible -> myGitCommit\n");
        if (current != NULL){
            free(current);
        }
        exit(EXIT_FAILURE);
    }

    //On récupère la référence de la tête et de la branche en paramètre
    char * hashHEAD = getRef("HEAD");
    char * hashBranch_name = getRef(branch_name);

    int first_commit = 0;

    //Si il s'agit du premier commit
    if (hashHEAD == NULL && hashBranch_name == NULL){
        printf("Les deux références sont NULL, on suppose qu'il s'agit du premier commit -> myGitCommit\n");
        first_commit = 1;
    }

    //Si l'un des hash seulement est NULL
    if ( (hashHEAD == NULL || hashBranch_name == NULL) && first_commit == 0){
        printf("L'une des deux branches pointe vers aucun commit -> myGitCommit\n");
        exit(EXIT_FAILURE);
    }

    //Si aucun des deux n'est NULL
    if (first_commit == 0){

        //Si la ref HEAD ne pointe pas sur le même commit que la branche
        if (strcmp(hashHEAD,hashBranch_name) != 0){
            printf("HEAD doit pointer sur le dernier commit de la branche -> (myGitCommit)\n");
            free(hashHEAD);
            free(hashBranch_name);
            return;
        }
    }
    
    //On récupère le WorkTree du fichier .add
    WorkTree * WT = ftwt(".add");

    //On supprime le fichier .add
    system("rm .add");

    //Si le WorkTree récupéré du fichier .add est NULL
    if (WT == NULL){

        //On libère la mémoire allouée
        free(hashHEAD);
        free(hashBranch_name);
        printf("Aucun fichier n'étaient présent dans le fichier .add -> (myGitCommit)\n");
        return;
    }
    
    //On save le WorkTree récupéré et on crée le commit associé
    char * hashWorkTree = saveWorkTree(WT,".");
    Commit * c = createCommit(hashWorkTree);

    //Si la référence est une chaine de caractère vide
    if (hashBranch_name != NULL){

        if (strlen(hashBranch_name) == 0){
            printf("Le hash dans le fichier %s est vide -> (myGitCommit)\n",branch_name);
        }

        //Sinon on ajoute une paire predecessor-hash au commit
        else{
            commitSet(c,"predecessor",hashBranch_name);
        }
    }

    //Si le message est NULL on le signale simplement
    if (message == NULL){
        printf("Le message est NULL -> (myGitCommit)\n");
    }

    //Si le message n'est pas NULL on ajoute une paire message-"message"
    else{
        commitSet(c,"message",message);
    }

    //On crée un instantané du commit
    char * hashCommit = blobCommit(c);
    printf("hashCommit -> %s\n",hashCommit);

    //On update la référence branch_name et de la tête
    createUpdateRef(branch_name,hashCommit);
    createUpdateRef("HEAD",hashCommit);

    //On libère la mémoire allouée si elle l'est toujours
    if (hashHEAD != NULL){
        free(hashHEAD);
    }

    //On libère la mémoire allouée si elle l'est toujours
    if (hashBranch_name != NULL){
        free(hashBranch_name);
    }

    //On libère la mémoire allouée
    free(current);
    free(hashWorkTree);
    free(hashCommit);
    freeWorkTree(WT);
    freeCommit(c);
}

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

    //Récupère le path vers la branche
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

    //Si la chaine est NULL, on ne peut pas créer de nouvelles branches
    if (chaine == NULL){
        printf("On ne peut pas créer de branches si il n'y a eu aucun commit au départ !\n");
        exit(EXIT_FAILURE);
    }

    //On update la branche avec le dernier commit
    createUpdateRef(branch,chaine);

    //On libère la mémoire allouée
    free(chaine);
}

//Renvoie la branche courante
char* getCurrentBranch(){

    //Si le fichier .current_branch n'existe pas
    if (file_exists(".current_branch") == 0){
        printf("Fichier .current_branch non-initialisé -> getCurrentBranch\n");
        exit(EXIT_FAILURE);
    }

    //On ouvre le fichier en mode lecture
    FILE * f = fopen(".current_branch","r");

    //Si il y a eu un problème d'ouverture
    if (f == NULL){
        printf("Erreur lors de l'ouverture du fichier -> getCurrentBranch\n");
        return NULL;
    }

    //On alloue une chaine temporaire pour récupérer la branche courante
    char * buff = malloc(sizeof(char)*300);
    strcpy(buff,"");

    //On récupère le nom de la branche dans le fichier
    if (fgets(buff,300,f) == NULL){
        printf("Le fichier .current_branch est vide -> getCurrentBranch\n");
        return NULL;
    }
    
    //Si il y a un saut à la lign en trop, on l'enlève en le remplaçant par un 0
    if( buff[strlen(buff)-1] == '\n' ){
        buff[strlen(buff)-1] = 0;
    }

    //On ferme le fichier
    fclose(f);

    return buff;
}

//Affiche la liste des commit d'une branche
void printBranch(char * branch){

    //Si la branche n'existe pas
    if (branchExists(branch) == 0){
        printf("La branche %s n'existe pas -> printBranch\n",branch);
        exit(EXIT_FAILURE);
    }

    //On récupère le hash de la branche
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

    //On libère la mémoire allouée
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

    //On récupère la path vers le commit, puis le commit lui-même
    char * path = hashToPathCommit(commit_hash);
    Commit * c = ftc(path);

    char * commit_pred = commit_hash;

    //On parcourt notre liste de commit
    while(c != NULL){

        //On insère dans la liste la valeur
        Cell * C1 = buildCell(commit_pred);

        if (strcmp(commit_pred,commit_hash) != 0){
            free(commit_pred);
        }
        
        insertFirst(L,C1);

        //Si le prédécessor existe on actualise la variable itérative
        commit_pred = commitGet(c,"predecessor");

        char * commit_hash_old = commitGet(c,"predecessor_old");

        //Si le commit possède un predecessor old, il est issu d'une fusion et donc on fait un appel récursif sur ce commit
        if (commit_hash_old != NULL){
            List * old = branchList(commit_hash_old);
            free(commit_hash_old);
            fusionList(&L,old);
            free(old);
        }

        char * commit_hash_current = commitGet(c,"predecessor_current");

        //Si le commit possède un predecessor current, il est issu d'une fusion et donc on fait un appel récursif sur ce commit
        if (commit_hash_current != NULL){
            List * current = branchList(commit_hash_current);
            free(commit_hash_current);
            fusionList(&L,current);
            free(current);
        }

        //Si le commit précédent n'est pas NULL
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

    if (c != NULL){
        freeCommit(c);
    }

    free(path);

    return L;
}

//Récupère le commit de toutes les branches
List * getAllCommits(){

    //On récupère la liste des branches
    List * L = initList();
    List * content = listdir(".refs");

    //On parcourt cette liste
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

        //On récupère le hash du dernier commit de la branche
        char * commit_hash = getRef(tmp);

        //On récupère la liste de tout les commits de la branche
        List * list = branchList(commit_hash);
        free(tmp);
        
        //Si le commit_hash n'est pas NULL, on libère la mémoire allouée associé
        if(commit_hash != NULL){
            free(commit_hash);
        }

        //De même pour la liste
        if (*list == NULL){
            free(list);
            continue;
        }

        Cell * cell = *list;

        //On parcourt la liste
        while(cell != NULL){

            //Si on  a pas trouvé le hash, on le rajoute à la liste pour éviter les doublons
            if(searchList(L,cell->data) == NULL){
                Cell * c = buildCell(cell->data);
                insertFirst(L,c);
            }

            cell = cell->next;
            
        }
        //On libère la liste temporaire
        FreeList(list);
    }

    //On libère la liste des branches
    FreeList(content);

    return L;
}

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
    
    //Si le hash est NULL
    if (hash == NULL){

        //On récupère la branche courante, le hash de cette branche, et on update la branche en paramètre avec ce hash
        char * current = getCurrentBranch();
        char * hash_c = getRef(current);
        createUpdateRef(branch,hash_c);

        //On libère la mémoire allouée
        free(head);
        free(hash);
        free(current);
        free(hash_c);
        return;
    }

    //On update la tête
    createUpdateRef("HEAD",hash);

    //On restaure les fichiers comme ils étaient lors du dernier commit
    restoreCommit(hash);

    //On libère la mémoire allouée
    free(head);
    free(hash);
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

    //On filtre la liste de tous les commit et on récupère la taille de la liste
    List * Commit_Pattern = filterList(AllCommits,pattern);
    int longueur = tailleList(Commit_Pattern);

    //Si la liste récupéré est NULL
    if (longueur < 1){

        //On libère la mémoire allouée
        FreeList(AllCommits);
        FreeList(Commit_Pattern);
        printf("Votre recherche de commit n'a pas abouti, Commit_Pattern est NULL -> myGitCheckout\n");
        exit(EXIT_FAILURE);
    }
    
    //Si la taille de la liste est supérieure à 1, il y a plusieurs commit, on les affiche et on demande à l'utilisateur de préciser sa recherche
    if (longueur > 1){
        //Afficher toute la liste ici
        char * list_commit_pattern = ltos(Commit_Pattern);
        printf("Liste des commits correspondant au pattern : \n%s\n",list_commit_pattern);

        //On libère la mémoire allouée
        free(list_commit_pattern);
        FreeList(AllCommits);
        FreeList(Commit_Pattern);

        printf("Votre pattern n'est pas assez précis, plusieurs possibilités : veuillez réessayer -> myGitCheckout\n");
        exit(EXIT_FAILURE);
    }

    //Si il n'y a qu'un seul commit dans la liste, on récupère son hash, on update la tête et on le restaure
    char * hash_commit = Commit_Pattern[0]->data;
    createUpdateRef("HEAD",hash_commit);
    restoreCommit(hash_commit);
    char * head = getRef("HEAD");

    //On crée une chaine de caractère qui signale qu'on est sur un commit
    char buff1[100]= "HEAD detached at ";
    for(int i=0;i<7;i++){
        buff1[i+17]=head[i];
    }
    
    //On l'insère dans .current-branch
    char buff2[300];
    sprintf(buff2,"echo %s > .current_branch",buff1);
    system(buff2);

    //On libère la mémoire allouée
    free(head);
    FreeList(AllCommits);
    FreeList(Commit_Pattern);
}