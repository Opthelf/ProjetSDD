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

#define TAILLE 10

//Crée une instance de la structure kvp, initialisé avec key et value
kvp * createKeyVal(char *key,char *val){

    //Teste si l'un des paramètres est NULL
    if (key == NULL){
        printf("La clé est NULL ce qui va créer une segmentation fault -> createKeyVal\n");
        exit(EXIT_FAILURE);
    }

    //On alloue la mémoire nécessaire pour stocker la clé et la valeur
    kvp * k = malloc(sizeof(kvp));

    //On récupère la clé
    k->key = strdup(key);

    //Si la valeur est NULL on lui met le champ (null)
    if (val == NULL){
        k->value = malloc(sizeof(char)*7);
        strcpy(k->value,"(null)");
    }

    //Sinon on lui donne juste la valeur
    else{
        k->value = strdup(val);
    }

    return k;
}

//Libère la mémoire du paramètre
void freeKeyVal(kvp *kv){

    //Teste si kv est NULL pour éviter une segmentation fault
    if (kv == NULL){
        printf("La paire key-value est NULL, pas besoin de la libérer -> freeKeyVal\n");
        return;
    }

    //Libère la mémoire
    free(kv->key);
    free(kv->value);
    free(kv);
}

//Convertit le paramètre en chaine de caractère
char * kvts(kvp* k){

    //Teste si l'un des paramètres est NULL
    if (k == NULL){
        printf("Le paramètre est NULL ce qui va créer une segmentation fault -> kvts\n");
        exit(EXIT_FAILURE);
    }

    //On alloue la mémoire nécessaire
    char *buff = malloc(sizeof(char)*100);

    //On récupère les données sous forme de chaine de caractères
    sprintf(buff,"%s : %s",k->key,k->value);
    strcat(buff,"\0");

    return buff;
}

//Convertit une chaine de caractère en key-value strcuture
kvp *stkv(char * ch){

    //Teste si la chaine est NULL
    if (ch == NULL){
        printf("La chaine de caractère est NULL -> stkv\n");
        exit(EXIT_FAILURE);
    }

    //Teste si la chaine est vide
    if (strlen(ch) == 0){
        printf("La chaine de caractère est vide -> stkv\n");
        exit(EXIT_FAILURE);
    }

    //récupère les clé et valeurs
    char key[100],val[100];
    sscanf(ch,"%s : %s",key,val);

    return createKeyVal(key,val);
}

//Initialise un Commit
Commit* initCommit(){

    //On alloue la mémoire nécessaire
    Commit * c = malloc(sizeof(Commit));

    //On initialise les champs
    c->T = malloc(sizeof(kvp*)*TAILLE);
    c->size = TAILLE;

    //On parcourt le tableau de key-value pour initialiser à NULL
    for(int i = 0 ; i < c->size ; i++){
        c->T[i] = NULL;
    }

    //On initialise le nombre d'éléments à 0
    c->n = 0;

    return c;
}

//Libère la mémoire d'un commit
void freeCommit(Commit *c){

    //Teste si le commit est NULL pour éviter une segmentation fault
    if (c == NULL){
        printf("Pas besoin de free le commit, il est NULL -> freeCommit\n");
        return;
    }

    //On parcourt le tableau
    for(int i = 0 ; i < c->size ; i++){

        //Si il n'est pas égal à NULL on le free
        if(c->T[i] != NULL){
            freeKeyVal(c->T[i]);
        }
    }

    //On libère la mémoire du tableau et de la structure
    free(c->T);
    free(c);
}

//Rajoute une paire key-value dans le commit
void commitSet(Commit *c, char *key, char *val){
    
    //Teste si le commit est NULL
    if (c == NULL){
        printf("Le commit c est NULL -> commitSet\n");
        exit(EXIT_FAILURE);
    }
    
    //Teste si la clé est NULL
    if (key == NULL){
        printf("La clé est NULL ce qui va créer une segmentation fault -> commitSet\n");
        exit(EXIT_FAILURE);
    }

    //On hash la clé pour récupérer la position où mettre la valeur
    int p = hash(key)%c->size;

    //On parcourt le tableau à partir de cette position
    while(c->T[p] != NULL){
        p = (p+1)%c->size;
    }

    //On crée la key-value avec key et val et on initialise le champs du commit dans le tableau
    c->T[p] = createKeyVal(key,val);
    c->n++;
}

//Crée un commit avec une paire tree-hash
Commit * createCommit(char* hash){

    //Si le hash est NULL, il y a un problème
    if (hash == NULL){
        printf("Le hash est NULL -> createCommit\n");
        exit(EXIT_FAILURE);
    }

    //On intialise le commit et on lui met la key-value tree-hash
    Commit * c = initCommit();
    commitSet(c,"tree",hash);

    return c;
}

//Récupère la valeur de clé key dans le commit c
char * commitGet(Commit *c,char *key){

    //Teste si le commit est NULL
    if (c == NULL){
        printf("Le commit c est NULL -> commitGet\n");
        exit(EXIT_FAILURE);
    }
    
    //Teste si l'un des paramètres est NULL
    if (key == NULL){
        printf("La clé est NULL ce qui va créer une segmentation fault -> commitGet\n");
        exit(EXIT_FAILURE);
    }

    //On hash la clé
    int p = hash(key)%c->size;
    int attempt = 0;

    //On parcout le tableau tant que on a des valeurs non nulles et qu'on a pas fait plus d'essai que la taille du tableau
    while(c->T[p] != NULL && attempt < c->size){

        //Si il s'agit de la bonne clé, on retourne la valeur associé
        if(strcmp(c->T[p]->key,key) == 0){

            char * value = strdup(c->T[p]->value);
            return value;
        }

        //Sinon on continue d'essayer
        p = (p+1)%c->size;
        attempt++;
    }
    return NULL;
}

//Transforme un commit en chaine de caractère
char * cts(Commit *c){

    //Teste si le commit est NULL
    if (c == NULL){
        printf("Le commit est NULL -> cts\n");
        exit(EXIT_FAILURE);
    }

    //Alloue la chaine de caractère qui va récupérer le commit
    char * res = malloc(sizeof(char)*100*c->n);
    strcpy(res,"");
    int count = 0;

    //On parcourt le tableau de commit
    for(int i = 0 ; i < c->size ; i++){

        //Si la valeur du tableau n'est pas NULL
        if(c->T[i] != NULL){
            count ++;

            //On récupère la chaine associé à la case du tableau
            char * temp = kvts(c->T[i]);

            //On l'ajoute à la chaine à retourner
            strcat(res,temp);

            free(temp);

            //Si jamais ce n'est pas la dernière chaine à récupérer on ajoute un retour à la ligne
            if(count < c->n){
                strcat(res,"\n");
            }
        }
    }

    //On ferme la chaine de caractères
    strcat(res,"\0");

    return res;
}

//Transforme une chaine de caractère en commit
Commit * stc(char *str){

    //Teste si la chaine est NULL
    if (str == NULL){
        printf("La chaine de caractère est NULL -> stc\n");
        exit(EXIT_FAILURE);
    }

    //Teste si la chaine de caractère est vide
    if ( strlen(str) == 0){
        printf("La chaine de caractère est vide -> stc\n");
        exit(EXIT_FAILURE);
    }

    //On initialise le commit
    Commit * c = initCommit();

    //On alloue l'espace mémoire pour la clé et la valeur
    char * reskey = malloc(sizeof(char)*100);
    strcpy(reskey,"");
    char * resvalue = malloc(sizeof(char)*100);
    strcpy(resvalue,"");

    int i = 0;

    //Tant que la chaine n'arrive pas à la fin
    while(str-1!=NULL){
        
        sscanf(str,"%s : %s",reskey,resvalue);
        strcat(reskey,"\0");

        strcat(resvalue,"\0");
       
        commitSet(c,reskey,resvalue);
        str = strchr(str,'\n')+1;
    }

    //On libère la mémoire allouée
    free(reskey);
    free(resvalue);

    return c;
}

//Ecrit la chaine de caractère d'un commit dans un fichier
void ctf(Commit *c,char *file){

    //On récupère l'état du fichier
    int statut_file = isFile(file);

    //Teste si le fichier est un dossier
    if (statut_file == 0){
        printf("Le fichier %s est un dossier, on ne peut pas y écrire le commit -> ctf\n",file);
        exit(EXIT_FAILURE);
    }

    //Ouvre le fichier en mode écriture
    FILE * f = fopen(file,"w");

    //Si il y a eu un problème d'ouverture
    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier %s -> ctf\n",file);
        exit(EXIT_FAILURE);
    }

    //On convertit le commit en chaine de caractères
    char * str = cts(c);

    //On la met dans le fichier
    fprintf(f,"%s",str);

    //On libère la mémoire allouée
    free(str);
    fclose(f);    
}

//Récupère un commit depuis un fichier
Commit * ftc(char *file){

    //récupère l'état d'un fichier
    int statut_file = isFile(file);

    //Teste si le fichier n'existe pas
    if (statut_file == -1){
        printf("Le fichier %s n'existe pas -> ftc\n",file);
        exit(EXIT_FAILURE);
    }

    //Teste si le fichier est un dossier
    if (statut_file == 0){
        printf("Le fichier %s est un dossier, on ne peut en récupérer un commit -> ftc\n",file);
        exit(EXIT_FAILURE);
    }

    //Ouvre le fichier en mode lecture
    FILE * f = fopen(file,"r");

    //Teste si on a réussi à ouvrir le fichier
    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier %s -> ftc\n",file);
        exit(EXIT_FAILURE);
    }

    //On alloue la mémoire pour stocker la chaine du commit associé
    char * str = malloc(sizeof(char)*10000);
    strcpy(str,"");

    //On alloue une chaine temporaire
    char * buff = malloc(sizeof(char)*100);
    strcpy(buff,"");

    //Parcourt le fichier ligne par ligne et récupère chaque chaine de carctère
    while(fgets(buff,1000,f)!=NULL){
        strcat(str,buff);
    }

    //Libère la mémoire de la chaine temporaire et ferme le fichier
    free(buff);
    fclose(f);
  
    //Récupère le commit associé à la chaine de caractères
    Commit * c = stc(str);

    //Libère la mémoire de la chaine finale
    free(str);
    
    return c;
}

//Crée un instantané du commit c dans le path de son hash
char * blobCommit(Commit *c){

    //Teste si le commit est NULL
    if (c == NULL){
        printf("Le commit en paramètre est NULL -> blobCommit\n");
        exit(EXIT_FAILURE);
    }

    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);
    
    //écrit le commit dans le fichier temporaire
    ctf(c,fname);

    //Récupère le hash de ce commit
    char * hash = sha256file(fname);

    //Récupère le path vers ce commit
    char * ch = hashToFile(hash);
    strcat(ch,".c");

    //Copie ce fichier dans le path
    cp(ch,fname);

    //Supprime le fichier temporaire
    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);

    //Libère la mémoire allouée
    free(ch);
    
    return hash;
}