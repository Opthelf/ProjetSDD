#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#define TAILLE 10


WorkFile* createWorkFile(char* name){ //Crée un WorkFile et initialise son hash à NULL et son nom à name et le mode à 0
    if (name == NULL){ //Teste si name est NULL
        printf("Le nom du fichier est NULL -> createWorkFile\n");
        exit(EXIT_FAILURE);
    }
    WorkFile * WF = (WorkFile *)malloc(sizeof(WorkFile));
    WF->name = strdup(name);
    WF->hash = NULL;
    WF->mode = getChmod(name);
    return WF;
}

void freeWorkFile(WorkFile * WF){ //Libère un WorkFile de la mémoire
    if(WF == NULL){
        printf("WorkFile NULL(freeWorkFile)\n");
        free(WF);
        return;
    }
    if(WF->name != NULL){
        free(WF->name);
    }
    if(WF->hash != NULL){
        free(WF->hash);
    }
    free(WF);
}

char* wfts(WorkFile* wf){ //Transforme un WokrFile sous forme de chaine de caractère
    if (wf == NULL){ //Teste si le WorkFile est NULL
        printf("Le WorkFile est NULL -> wfts\n");
        exit(EXIT_FAILURE);
    }

    char * res = malloc(1000*sizeof(char));
    if (wf->hash == NULL){
        sprintf(res,"%s\tnull\t%d",wf->name,wf->mode);
    }
    else{
        sprintf(res,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
    }
    res[strlen(res)] = '\0';
    return res;
}

WorkFile* stwf(char* ch){ //Transforme une chaine de caractère en un WokrTree
    int mode;
    char * name = malloc(sizeof(char)*1000);
    char * hash = malloc(sizeof(char)*1000);
    sscanf(ch,"%s\t%s\t%d",name,hash,&mode);

    if (strlen(name) == 0){ //Teste si la chaine de name est vide
        printf("Le nom est vide -> stwf\n");
        exit(EXIT_FAILURE);
    }

    WorkFile * WF = createWorkFile(name);
    WF->hash = strdup(hash);
    WF->mode = mode;

    free(hash);
    free(name);
    return WF;
}

WorkTree * initWorkTree(){ //Initialise un WorkTree
    WorkTree * WT = (WorkTree *) malloc(sizeof(WorkTree));
    WT->size = TAILLE;
    WT->tab = (WorkFile*) malloc(TAILLE * sizeof(WorkFile));
    WT->n = 0;
    return WT;
}

void freeWorkTree(WorkTree* wt){ //Libère un WorkTree de la mémoire
    int i = 0;
    while(i < wt->n){
        free(wt->tab[i].hash);
        free(wt->tab[i].name);
        i++;
    }
    free(wt->tab);
    free(wt);
}


int inWorkTree(WorkTree* wt, char* name){ //Vérifie si le fichier name est déjà enregistré dans le WorkTree et renvoie sa position si il y est
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL(inWorkTree)\n");
        exit(EXIT_FAILURE);
    }

    WorkFile * WF = wt->tab;
    int i = 0;
    while(i < (wt->n)){
        if (strcmp(name,WF[i].name) == 0){
            return i;
        }
        i++;
    }
    //printf("Le fichier passé en paramètre n'est pas dans le WorkTree(inWorkTree)\n");
    return -1;
}

//Transforme un hash en path
char * hashToPathWorkTree(char * hash){

    //On transforme le hash en path
    char * buff = hashToPath(hash);
    buff = realloc(buff, sizeof(char) * (strlen(buff)+3) );

    //On ajoute le .t des instantanés de WorkTree
    strcat(buff,".t");
    strcat(buff,"\0");
    return buff;
}

int appendWorkTree(WorkTree* wt, char * n, char * h, int m){ //Rajoute un WorkFile au WorkTree en paramètre
    
    int etat_WT = inWorkTree(wt,n); 

    //Cas où wt est NULL traité dans inWorkTree
    if ( etat_WT != -1){
        //printf("Le fichier %s est déjà dans le WorkTree !\n",n);
        return 0;
    }

    
    if (wt->n < wt->size){

        wt->tab[wt->n].mode = m;
        
        wt->tab[wt->n].name = strdup(n);

        // test NULL hash
        if(h != NULL ){
            wt->tab[wt->n].hash = strdup(h);
        }
        else{
            wt->tab[wt->n].hash = NULL;
        }
        wt->n += 1;
        
        //free(n); // !!!Attention ça peut libérer des chaines en dehors de la fonction!!!
        //free(h); // !!!Attention ça peut libérer des chaines en dehors de la fonction!!!
        return 1;
    }
    printf("Le WorkTree est à sa taille maximale, le fichier %s n'a donc pas pu être ajouté\n",n);
    return 0;
}

void afficheWT(WorkTree* wt){
    int i = 0;
    while(i < wt->n){
        printf("%s\t%s\t%d\n",wt->tab[i].name,wt->tab[i].hash,wt->tab[i].mode);
        i++;
    }
}

char* wtts(WorkTree* wt){ //Transforme un WorkTree en une chaine de caractère
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL(wtts)\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char * res = (char*)malloc(sizeof(char)*1000);
    strcpy(res,"");
    while(i < wt->n){
        char *buff = wfts(&(wt->tab[i]));
        strcat(res,buff);
        if (i != wt->n-1){
            strcat(res,"\n");
        }
        i++;
        free(buff);
    }
    strcat(res,"\0");
    return res;
}

WorkTree* stwt(char* ch){ //Transforme une chaine de caractère en un WorkTree
    if (ch == NULL){ //Teste si la chaine est NULL
        printf("La chaine est NULL -> stwt\n");
        exit(EXIT_FAILURE);
    }
    
    if (strlen(ch) == 0){ //Teste si la chaine est vide
        printf("La chaine est vide -> stwt\n");
        //exit(EXIT_FAILURE); Je ne sais pas lequel des deux vaut mieux ici
        return NULL;
    }

    WorkTree * WT= initWorkTree();
    char * resname = malloc(sizeof(char)*1000);
    char * reshash = malloc(sizeof(char)*1000);
    int resmode = 0;
    while(ch -1 != NULL){ //ch -1 et usage de strchr unclear mais fonctionnel en apparence ici
        sscanf(ch,"%s\t%s\t%d",resname,reshash,&resmode);
        strcat(resname,"\0");
        strcat(reshash,"\0");
        appendWorkTree(WT,resname,reshash,resmode);
        ch = strchr(ch,'\n')+1;
    }
    free(resname);
    free(reshash);
    return WT;

}

int wttf(WorkTree* wt, char* file){ //Transforme un WorkTree en une chaine de carctère puis la met dans un fichier
    
    int etat_file = isFile(file);
    
    if (etat_file == 0){
        printf("Le fichier %s est un répertoire -> wttf\n",file);
        exit(EXIT_FAILURE);
    }

    FILE * f = fopen(file,"w");
    if (f != NULL){
        char * chaine = wtts(wt);
        fprintf(f,"%s",chaine);
        fclose(f);
        free(chaine);
        return 1;
    }
    printf("Problème d'ouverture du fichier %s -> (wttf)\n",file);
    return 0;
}

WorkTree* ftwt(char* file){ //Récupère un WorkTree d'un fichier
    int etat_file = isFile(file);
    if (etat_file == -1){
        printf("Le fichier %s n'existe pas -> ftwt\n",file);
        return NULL;
    }
    if (etat_file == 0){
        printf("Le fichier %s est un répertoire -> ftwt\n",file);
        return NULL;
    }
    
    FILE * f = fopen(file,"r");
    if (f == NULL){
        printf("Problème d'ouverture du fichier %s -> (ftwt)\n",file);
        exit(EXIT_FAILURE);
    }

    char * buff = malloc(sizeof(char)*1000);
    strcpy(buff,"");
    char * chaineWT = malloc(sizeof(char)*1000*TAILLE);
    strcpy(chaineWT,"");
    while(fgets(buff,1000,f) != NULL){
        strcat(chaineWT,buff);
    }
    WorkTree * WT = stwt(chaineWT);
    free(buff);
    free(chaineWT);
    fclose(f);
    return WT;
}