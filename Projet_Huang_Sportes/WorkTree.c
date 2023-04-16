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


//Crée un WorkFile et initialise son hash à NULL et son nom à name et le mode à 0
WorkFile* createWorkFile(char* name){

    //Teste si name est NULL
    if (name == NULL){
        printf("Le nom du fichier est NULL -> createWorkFile\n");
        exit(EXIT_FAILURE);
    }

    WorkFile * WF = (WorkFile *)malloc(sizeof(WorkFile));
    WF->name = strdup(name);
    WF->hash = NULL;
    WF->mode = getChmod(name);

    return WF;
}

//Libère un WorkFile de la mémoire
void freeWorkFile(WorkFile * WF){

    //Si le WorkFile est NULL
    if(WF == NULL){
        printf("WorkFile NULL(freeWorkFile)\n");
        free(WF);
        return;
    }

    //Si le nom du WorkFile n'est pas NULL
    if(WF->name != NULL){
        free(WF->name);
    }

    //Si le hash du WorkFile n'est pas NULL
    if(WF->hash != NULL){
        free(WF->hash);
    }

    free(WF);
}

//Transforme un WokrFile sous forme de chaine de caractère
char* wfts(WorkFile* wf){

    //Teste si le WorkFile est NULL
    if (wf == NULL){
        printf("Le WorkFile est NULL -> wfts\n");
        exit(EXIT_FAILURE);
    }

    //On alloue la chaine de caractère que l'on va retourner
    char * res = malloc(1000*sizeof(char));

    //Si le hash est NULL
    if (wf->hash == NULL){
        sprintf(res,"%s\tnull\t%d",wf->name,wf->mode);
    }
    
    //Sinon
    else{
        sprintf(res,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
    }

    //On ferme la chaine de caratères
    res[strlen(res)] = '\0';

    return res;
}

//Transforme une chaine de caractère en un WokrTree
WorkFile* stwf(char* ch){
    int mode;

    //On alloue les chaines de caractères correspondant aux du WorkFile
    char * name = malloc(sizeof(char)*1000);
    char * hash = malloc(sizeof(char)*1000);

    sscanf(ch,"%s\t%s\t%d",name,hash,&mode);

    //Teste si la chaine de name est vide
    if (strlen(name) == 0){
        printf("Le nom est vide -> stwf\n");
        exit(EXIT_FAILURE);
    }

    //On crée et initialise le WorkFile
    WorkFile * WF = createWorkFile(name);
    WF->hash = strdup(hash);
    WF->mode = mode;

    //On libère la mémoire allouée
    free(hash);
    free(name);

    return WF;
}

//Initialise un WorkTree
WorkTree * initWorkTree(){

    //Alloue le WorkTree
    WorkTree * WT = (WorkTree *) malloc(sizeof(WorkTree));

    //Initialise et alloue ses champs
    WT->size = TAILLE;
    WT->tab = (WorkFile*) malloc(TAILLE * sizeof(WorkFile));
    WT->n = 0;

    return WT;
}

//Libère un WorkTree de la mémoire
void freeWorkTree(WorkTree* wt){
    int i = 0;

    //On parcourt le WorkTree
    while(i < wt->n){

        //On libère le hash et le nom du WorkFile courant
        free(wt->tab[i].hash);
        free(wt->tab[i].name);

        i++;
    }

    //On libère le tableau et la structure en ell-même
    free(wt->tab);
    free(wt);
}

//Vérifie si le fichier name est déjà enregistré dans le WorkTree et renvoie sa position si il y est
int inWorkTree(WorkTree* wt, char* name){

    //Teste si le WorkTree est NULL
    if (wt == NULL){
        printf("Le WorkTree est NULL(inWorkTree)\n");
        exit(EXIT_FAILURE);
    }

    WorkFile * WF = wt->tab;
    int i = 0;

    //On parcourt le WorkTree
    while(i < (wt->n)){

        //Si le nom du fichier et le nom du fichier dans le WokrFile sont les mêmes on retourne la position du WokrFile
        if (strcmp(name,WF[i].name) == 0){
            return i;
        }

        i++;
    }

    //Si le fichier passé en paramètre n'est pas dans le WorkTree
    return -1;
}

//Rajoute un WorkFile au WorkTree en paramètre
int appendWorkTree(WorkTree* wt, char * n, char * h, int m){
    
    //Cas où wt est NULL traité dans inWorkTree
    int etat_WT = inWorkTree(wt,n); 

    //Si le fichier est déjà dans le WorkTree
    if ( etat_WT != -1){
        return 0;
    }

    //Si la taille maximale est déjà atteint voir dépassée dans un cas improbable
    if (wt->n < wt->size){

        wt->tab[wt->n].mode = m;
        wt->tab[wt->n].name = strdup(n);

        //Si le hash est NULL
        if(h != NULL ){
            wt->tab[wt->n].hash = strdup(h);
        }

        //Sinon
        else{
            wt->tab[wt->n].hash = NULL;
        }

        //On actualise le nombre de fichier présent dans le WorkTree
        wt->n += 1;
        
        return 1;
    }

    //Si la taille maximale est atteinte, on prévient l'utilisateur
    printf("Le WorkTree est à sa taille maximale, le fichier %s n'a donc pas pu être ajouté\n",n);
    return 0;
}

//Affiche le WorkTree
void afficheWT(WorkTree* wt){
    int i = 0;

    //On parcourt le WorkTree
    while(i < wt->n){

        //On affiche le WorkFile associé
        printf("%s\t%s\t%d\n",wt->tab[i].name,wt->tab[i].hash,wt->tab[i].mode);
        i++;
    }
}

//Transforme un WorkTree en une chaine de caractère
char* wtts(WorkTree* wt){

    //Teste si le WorkTree est NULL
    if (wt == NULL){
        printf("Le WorkTree est NULL(wtts)\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    //Alloue la chaine que l'on va retourner
    char * res = (char*)malloc(sizeof(char)*1000);
    strcpy(res,"");

    //On parcourt le WorkTree
    while(i < wt->n){

        //On récupère la chaine associé au WorkFile et on la rajoute à la chaine finale
        char *buff = wfts(&(wt->tab[i]));
        strcat(res,buff);

        //Si ce n'est pas le dernier tour de boucle on rajoute un retour à la ligne à la chaine de caractères
        if (i != wt->n-1){
            strcat(res,"\n");
        }

        i++;
        free(buff);
    }

    //On ferme la chaine de caractère
    strcat(res,"\0");
    return res;
}

//Transforme une chaine de caractère en un WorkTree
WorkTree* stwt(char* ch){

    //Teste si la chaine est NULL
    if (ch == NULL){
        printf("La chaine est NULL -> stwt\n");
        exit(EXIT_FAILURE);
    }
    
    //Teste si la chaine est vide
    if (strlen(ch) == 0){
        printf("La chaine est vide -> stwt\n");
        return NULL;
    }

    WorkTree * WT= initWorkTree();

    //On alloue les chaines de caractères du nom et du hash
    char * resname = malloc(sizeof(char)*1000);
    char * reshash = malloc(sizeof(char)*1000);
    int resmode = 0;

    //Tant que la chaine de caractère n'est pas fini
    while(ch -1 != NULL){

        //On récupère le nom et le hash de la chaine
        sscanf(ch,"%s\t%s\t%d",resname,reshash,&resmode);
        strcat(resname,"\0");
        strcat(reshash,"\0");

        //On rajoute au WorkTree le WorkFile associé
        appendWorkTree(WT,resname,reshash,resmode);
        ch = strchr(ch,'\n')+1;
    }

    //On libère la mémoire allouée
    free(resname);
    free(reshash);

    return WT;

}

//Transforme un WorkTree en une chaine de carctère puis la met dans un fichier
int wttf(WorkTree* wt, char* file){
    
    //On récupère l'état du fichier
    int etat_file = isFile(file);
    
    //Si le fichier est un répertoire
    if (etat_file == 0){
        printf("Le fichier %s est un répertoire -> wttf\n",file);
        exit(EXIT_FAILURE);
    }

    //On ouvre le fichier en mode écriture
    FILE * f = fopen(file,"w");

    //Si il n'y a pas eu de problèmes d'ouverture
    if (f != NULL){

        //On récupère la chaine associée au WorkTree
        char * chaine = wtts(wt);

        //On l'insère dans le fichier
        fprintf(f,"%s",chaine);

        //On ferme le fichier
        fclose(f);

        //On libère la mémoire allouée
        free(chaine);

        return 1;
    }

    printf("Problème d'ouverture du fichier %s -> (wttf)\n",file);
    return 0;
}

//Récupère un WorkTree d'un fichier
WorkTree* ftwt(char* file){

    //On récupère l'état du fichier
    int etat_file = isFile(file);

    //Si le fichier n'existe pas
    if (etat_file == -1){
        printf("Le fichier %s n'existe pas -> ftwt\n",file);
        return NULL;
    }

    //Si le fichier est un dossier
    if (etat_file == 0){
        printf("Le fichier %s est un répertoire -> ftwt\n",file);
        return NULL;
    }
    
    //On ouvre le fichier en mode lecture
    FILE * f = fopen(file,"r");

    //Si ilm y a eu un problème d'ouverture du fichier
    if (f == NULL){
        printf("Problème d'ouverture du fichier %s -> (ftwt)\n",file);
        exit(EXIT_FAILURE);
    }

    //On alloue la chaine de caractère pour stocker chaque WorkFile
    char * buff = malloc(sizeof(char)*1000);
    strcpy(buff,"");

    //On alloue la chaine de caractères pour stocker le WorkTree
    char * chaineWT = malloc(sizeof(char)*1000*TAILLE);
    strcpy(chaineWT,"");

    //On récupère ligne par ligne les WorkFile
    while(fgets(buff,1000,f) != NULL){
        strcat(chaineWT,buff);
    }

    //On convertit la chaine de caractère en WorkTree
    WorkTree * WT = stwt(chaineWT);

    //On libère la mémoire allouée
    free(buff);
    free(chaineWT);

    //On ferme le fichier
    fclose(f);

    return WT;
}

//Crée un instantanée d'un WorkTree
char * blobWorkTree(WorkTree *wt){

    //Teste si le WorkTree est NULL
    if (wt == NULL){
        printf("Le WorkTree est NULL -> blobWorkTree\n");
       exit(EXIT_FAILURE);
    }

    //On crée le fichier temporaire
    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);

    //On stocke le WorkTree dans le fichier temporaire
    wttf(wt,fname);

    //On hash le fichier, récupère le path de ce hash, et le transforme en path de WorkTree
    char * hash = sha256file(fname);
    char * ch = hashToFile(hash);
    ch = realloc(ch,(strlen(ch)+3)*sizeof(char));
    strcat(ch,".t");

    //On copie le fichier temporaire dans le fichier dont le path est créé à partir du fichier temporaire
    cp(ch,fname);
    setMode(700,ch);

    //On supprime le fichier temporaire
    char remove[1000] = "rm ";
	strcat(remove,fname);
	system(remove);

    //On libère la mémoire allouée
    free(ch);

    return hash;
}

char * saveWorkTree(WorkTree *wt,char * path){ //La fonction sauvegarde les fichiers présents dans le WorkTree, dont la racine est à path
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL -> saveWorkTree\n");
       exit(EXIT_FAILURE);
    }

    if (path == NULL){ //Teste si le path est NULL
        printf("Le path est NULL -> saveWorkTree\n");
       exit(EXIT_FAILURE);
    }

    char * absPath;
    for(int i=0;i<wt->n;i++){ //Parcours des WorkFile présents

        //On récupère le path du WorkFile correspondant
        absPath = concat_paths(path,wt->tab[i].name);

        //On stocke le statut du fichier pour faire nos cas
        int etat_file = isFile(absPath);

        //Teste si le fichier n'existe pas
        if (etat_file == -1){
            printf("Le fichier %s n'existe pas à l'endroit indiqué (peut-être une erreur du paramètre path ?) -> saveWorkTree\n",absPath);
            free(absPath);
            exit(EXIT_FAILURE);
        }

        //Teste si le WorkFile est un fichier
        if (etat_file == 1){
           
            blobFile(absPath);
           
           //Si le hash du WorkFile est NULL
            if(wt->tab[i].hash == NULL){
                wt->tab[i].hash = sha256file(absPath);
                wt->tab[i].mode = getChmod(absPath);
                
            }

            //Sinon
            else{
                free(wt->tab[i].hash);
                wt->tab[i].hash = sha256file(absPath);
                wt->tab[i].mode = getChmod(absPath);
            }
        }

        //Teste si le WorkFile est un dossier
        if (etat_file == 0){
         
            WorkTree *wt2 = initWorkTree();

            //On récupère l'ensemble des fichiers et dossiers présents dans le dossier
            List * L = listdir(absPath);

            //On construit notre WorkTree
            for(Cell * ptr = *L;ptr !=NULL ; ptr = ptr->next){

                //Ignore les fichiers commençant par .
                if(ptr->data[0]=='.'){
                    continue;
                }

                char buff[1000];
                strcpy(buff,ptr->data);

                //On rajoute le WorkFile au WorkTree en construction
                appendWorkTree(wt2,buff,NULL,777);
            }

            //Si le hash du WorkFile est NULL on ne free pas le hash précédent, on sauvegarde le WorkTree et on initialise le hash et le mode
            if(wt->tab[i].hash == NULL){
                wt->tab[i].hash = saveWorkTree(wt2,absPath);
                wt->tab[i].mode = getChmod(absPath);
            }

            //Sinon le hash du WorkFile est NULL on free le hash précédent, on sauvegarde le WorkTree et on initialise le hash et le mode
            else{
                free(wt->tab[i].hash);
                wt->tab[i].hash = saveWorkTree(wt2,absPath);
                wt->tab[i].mode = getChmod(absPath);
            }

            //On libère la mémoire allouée
            FreeList(L);
            freeWorkTree(wt2);
        }
        free(absPath);
    }

    //On retourne le hash du WorkTree, tout en créant un instantané de celui-ci
    return blobWorkTree(wt);
}

//Vérifie si le hash en parmètre correspond au hash d'un WorkTree
int isWorkTree(char* hash){

    //Teste si le paramètre est NULL
    if (hash == NULL){
        printf("Le hash en paramètre est NULL -> isWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    char * path = hashToPath(hash);

    //Si le fichier existe sans le .t
    if (file_exists(path)){
        free(path);
        return 0; 
    }

    strcat(path,".t");

    //Si le fichier existe sous forme de .t
    if (file_exists(path)){ 
        free(path);
        return 1;
    }

    //On libère la mémoire allouée
    free(path);

    return -1; 
}

//Restore les fichiers tels qu'ils étaient dans wt
void restoreWorkTree(WorkTree * wt, char * path){

    //Teste si le WorkTree est NULL
    if (wt == NULL){
        printf("Le WorkTree est NULL -> restoreWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    //Teste si le path est NULL
    if (path == NULL){
        printf("Le path est NULL -> restoreWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    //On parcourt les WorkFile de wt
    for(int i = 0 ; i < wt->n ; i ++){

        //On récupère le path absolu vers l'endroit où l'on veut récupèrer le fichier
        char * absPath = concat_paths(path,wt->tab[i].name);

        //On récupère le path de copie de l'endroit d'où l'on copie le fichier
        char * copyPath = hashToPath(wt->tab[i].hash);

        //On récupère le hash du fichier que l'on veut copier
        char * hash = wt->tab[i].hash;

        int statut_WT = isWorkTree(wt->tab[i].hash);

        if (statut_WT == -1){
            printf("Le hash %s sous forme de path est introuvable\n",hash);
            exit(EXIT_FAILURE) ;
        }

        //Si il s'agit d'un WorkFile
        if (statut_WT == 0){
            
            //On copie le fichier et on met son mode au mode du fichier d'origine
            cp(absPath,copyPath);
            setMode(wt->tab[i].mode,absPath);
        }

        //Si il s'agit d'un WorkTree
        if (statut_WT == 1){
            
            //On récupère le WorkTree associé et on le restore, puis on met le mode du fichier
            copyPath = realloc(copyPath,strlen(copyPath)+3);
            strcat(copyPath,".t");
            strcat(copyPath,"\0");
            WorkTree * newwt = ftwt(copyPath);
            restoreWorkTree(newwt,absPath);
            setMode(getChmod(copyPath),absPath);
            freeWorkTree(newwt);
        }

        //On libère la mémoire associée
        free(absPath);
        free(copyPath);
    }
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