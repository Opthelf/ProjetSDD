#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#define TAILLE 10

//Done
kvp * createKeyVal(char *key,char *val){ //Crée une instance de la structure kvp, initialisé avec key et value

    if (key == NULL){ //Teste si l'un des paramètres est NULL
        printf("La clé est NULL ce qui va créer une segmentation fault -> createKeyVal\n");
        exit(EXIT_FAILURE);
    }

    kvp * k = malloc(sizeof(kvp));
    k->key = strdup(key);
    if (val == NULL){
        k->value = malloc(sizeof(char)*7);
        strcpy(k->value,"(null)");
    }
    else{
        
        k->value = strdup(val);
    }
    return k;
}

//Done
void freeKeyVal(kvp *kv){ //Libère la mémoire du paramètre
    if (kv == NULL){ //Teste si kv est NULL pour éviter une segmentation fault
        printf("La paire key-value est NULL, pas besoin de la libérer -> freeKeyVal\n");
        return;
    }

    free(kv->key);
    free(kv->value);
    free(kv);
}

//Done
char * kvts(kvp* k){ //Convertit le paramètre en chaine de caractère
    if (k == NULL){ //Teste si l'un des paramètres est NULL
        printf("Le paramètre est NULL ce qui va créer une segmentation fault -> kvts\n");
        exit(EXIT_FAILURE);
    }

    char *buff = malloc(sizeof(char)*100);
    sprintf(buff,"%s : %s",k->key,k->value);
    strcat(buff,"\0");
    return buff;
}

//Done
kvp *stkv(char * ch){ //Convertit une chaine de caractère en key-value strcuture
    if (ch == NULL){ //Teste si la chaine est NULL
        printf("La chaine de caractère est NULL -> stkv\n");
        exit(EXIT_FAILURE);
    }

    if (strlen(ch) == 0){ //Teste si la chaine est vide
        printf("La chaine de caractère est vide -> stkv\n");
        exit(EXIT_FAILURE);
    }

    char key[100],val[100];
    sscanf(ch,"%s : %s",key,val);
    return createKeyVal(key,val);
}

//Done
Commit* initCommit(){ //Initialise un Commit
    Commit * c = malloc(sizeof(Commit));
    c->T = malloc(sizeof(kvp*)*TAILLE);
    c->size = TAILLE;
    for(int i = 0 ; i < c->size ; i++){
        c->T[i] = NULL;
    }
    c->n = 0;
    return c;
}

//Done
void freeCommit(Commit *c){ //Libère la mémoire d'un commit
    if (c == NULL){ //Teste si le commit est NULL pour éviter une segmentation fault
        printf("Pas besoin de free le commit, il est NULL -> freeCommit\n");
        return;
    }
    for(int i = 0 ; i < c->size ; i++){
        if(c->T[i] != NULL){
            freeKeyVal(c->T[i]);
        }
    }
    free(c->T);
    free(c);
}

//Done
unsigned long hash(unsigned char *str){ //Hash le contenu
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}


//Done
void commitSet(Commit *c, char *key, char *val){ //Rajoute une paire key-value dans le commit
    
    if (c == NULL){ //Teste si le commit est NULL
        printf("Le commit c est NULL -> commitSet\n");
        exit(EXIT_FAILURE);
    }
    
    if (key == NULL){ //Teste si la clé est NULL
        printf("La clé est NULL ce qui va créer une segmentation fault -> commitSet\n");
        exit(EXIT_FAILURE);
    }

    int p = hash(key)%c->size;
    while(c->T[p] != NULL){
        p = (p+1)%c->size;
    }
    c->T[p] = createKeyVal(key,val);
    c->n++;
}

//Done
Commit * createCommit(char* hash){ //Crée un commit avec une paire tree-hash
    if (hash == NULL){
        printf("Le hash est NULL -> createCommit\n");
        exit(EXIT_FAILURE);
    }

    Commit * c = initCommit();
    commitSet(c,"tree",hash);
    return c;
}

//Done
char * commitGet(Commit *c,char *key){ //Récupère la valeur de clé key dans le commit c
    if (c == NULL){ //Teste si le commit est NULL
        printf("Le commit c est NULL -> commitGet\n");
        exit(EXIT_FAILURE);
    }
    
    if (key == NULL){ //Teste si l'un des paramètres est NULL
        printf("La clé est NULL ce qui va créer une segmentation fault -> commitGet\n");
        exit(EXIT_FAILURE);
    }

    int p = hash(key)%c->size;
    int attempt = 0;

    while(c->T[p] != NULL && attempt < c->size){
        if(strcmp(c->T[p]->key,key) == 0){ //Si il s'agit de la bonne clé, on retourne la valeur associé
            //printf("%s dans commitGet\n",c->T[p]->value);
            char * value = strdup(c->T[p]->value);
            return value;
        }
        p = (p+1)%c->size;
        attempt++;
    }
    return NULL;
}


//Done
char * cts(Commit *c){ //Transforme un commit en chaine de caractère
    if (c == NULL){ //Teste si le commit est NULL
        printf("Le commit est NULL -> cts\n");
        exit(EXIT_FAILURE);
    }

    char * res = malloc(sizeof(char)*100*c->n);
    strcpy(res,"");
    int count = 0;

    for(int i=0;i<c->size;i++){
        if(c->T[i] != NULL){
            count ++;
            char * temp = kvts(c->T[i]);
            strcat(res,temp);
            free(temp);
            if(count < c->n){
                strcat(res,"\n");
            }
        }
    }
    strcat(res,"\0");
    return res;
}

//Done
Commit * stc(char *str){ //Transforme une chaine de caractère en commit
    if (str == NULL){ //Teste si la chaine est NULL
        printf("La chaine de caractère est NULL -> stc\n");
        exit(EXIT_FAILURE);
    }

    if ( strlen(str) == 0){ //Teste si la chaine de caractère est vide
        printf("La chaine de caractère est vide -> stc\n");
        exit(EXIT_FAILURE);
    }

    Commit * c = initCommit();
    char * reskey = malloc(sizeof(char)*100);
    strcpy(reskey,"");
    char * resvalue = malloc(sizeof(char)*100);
    strcpy(resvalue,"");
    int i=0;
    while(str-1!=NULL){
        
        sscanf(str,"%s : %s",reskey,resvalue);
        strcat(reskey,"\0");
       

        strcat(resvalue,"\0");
       
        commitSet(c,reskey,resvalue);
        str = strchr(str,'\n')+1;
    }
    free(reskey);
    free(resvalue);
    return c;
}

//Done
void ctf(Commit *c,char *file){ //Ecrit la chaine de caractère d'un commit dans un fichier
    int statut_file = isFile(file);

    if (statut_file == 0){ //Teste si le fichier est un dossier
        printf("Le fichier %s est un dossier, on ne peut pas y écrire le commit -> ctf\n",file);
        exit(EXIT_FAILURE);
    }

    FILE * f = fopen(file,"w");
    if(f == NULL){
        printf("Erreur lors de l'ouverture du fichier %s -> ctf\n",file);
        exit(EXIT_FAILURE);
    }

    char * str = cts(c);
    fprintf(f,"%s",str);
    free(str);
    fclose(f);    
}


//Done
Commit * ftc(char *file){ //Récupère un commit depuis un fichier
    int statut_file = isFile(file);

    if (statut_file == -1){ //Teste si le fichier n'existe pas
        printf("Le fichier %s n'existe pas -> ftc\n",file);
        exit(EXIT_FAILURE);
    }

    if (statut_file == 0){ //Teste si le fichier est un dossier
        printf("Le fichier %s est un dossier, on ne peut en récupérer un commit -> ftc\n",file);
        exit(EXIT_FAILURE);
    }


    FILE * f = fopen(file,"r");

    if(f == NULL){ //Teste si on a réussi à ouvrir le fichier
        printf("Erreur lors de l'ouverture du fichier %s -> ftc\n",file);
        exit(EXIT_FAILURE);
    }

    char * str = malloc(sizeof(char)*10000);
    strcpy(str,"");
    char * buff = malloc(sizeof(char)*100);
    strcpy(buff,"");
    while(fgets(buff,1000,f)!=NULL){
        strcat(str,buff);
    }
    free(buff);
    fclose(f);
  
    Commit * c = stc(str);
    free(str);
    return c;
}

//Done
char * blobCommit(Commit *c){ //Crée un instantané du commit c dans le path de son hash
    if (c == NULL){ //Teste si le commit est NULL
        printf("Le commit en paramètre est NULL -> blobCommit\n");
        exit(EXIT_FAILURE);
    }

    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);
    
    ctf(c,fname);
    char * hash = sha256file(fname);
    char * ch = hashToFile(hash);
    strcat(ch,".c");
    cp(ch,fname);

    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);

    free(ch);
    return hash;
}