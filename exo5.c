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

char * blobWorkTree(WorkTree *wt){ //Crée un instantanée d'un WorkTree
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL -> blobWorkTree\n");
       exit(EXIT_FAILURE);
    }

    //On crée le fichier temporaire
    char fname[100]= "myfileXXXXXX"; //avec le /tmp/ à la fac.
    int fd = mkstemp(fname);

    //On stocke le WorkTree dans le fichier temporaire
    wttf(wt,fname);

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

    free(ch);
    return hash;
}


char * concat_paths(char * path1,char * path2){ //Concatène les deux path entre eux, en mettant entre le premier et le deuxième un '\'
    if(path1 == NULL || path2 == NULL){
        printf("L'un des deux path, voir les deux sont NULL -> concat_paths\n");
       exit(EXIT_FAILURE);
    }

    int len = strlen(path1)+ strlen(path2)+2;

    char * result = malloc(len*sizeof(char));
    if (result == NULL){
        printf("Erreur d'allocation mémoire\n");
       exit(EXIT_FAILURE);
    } 
    char * cp1=strdup(path1);
    char * cp2=strdup(path2);
    strcpy(result,"");

    strcat(result,cp1);
    strcat(result,"/");
    strcat(result,cp2);
    strcat(result,"\0");
    free(cp1);
    free(cp2);
    return result;
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

        if (etat_file == -1){ //Teste si le fichier n'existe pas
            printf("Le fichier %s n'existe pas à l'endroit indiqué (peut-être une erreur du paramètre path ?) -> saveWorkTree\n",absPath);
            free(absPath);
            exit(EXIT_FAILURE);
        }

        if (etat_file == 1){ //Teste si le WorkFile est un fichier
           
            blobFile(absPath);
           
            if(wt->tab[i].hash == NULL){
                 wt->tab[i].hash = sha256file(absPath);
                 wt->tab[i].mode = getChmod(absPath);
                 

            }
            else{
                free(wt->tab[i].hash);
                wt->tab[i].hash = sha256file(absPath);
                wt->tab[i].mode = getChmod(absPath);
            }
        }

        if (etat_file == 0){ //Teste si le WorkFile est un dossier
         
            WorkTree *wt2 = initWorkTree();

            //On récupère l'ensemble des fichiers et dossiers présents dans le dossier
            List * L = listdir(absPath);

            for(Cell * ptr = *L;ptr !=NULL ; ptr = ptr->next){ //On construit notre WorkTree
                if(ptr->data[0]=='.'){ //Ignore les fichiers commençant par .
                    continue;
                }
                char buff[1000];
                strcpy(buff,ptr->data);
                appendWorkTree(wt2,buff,NULL,777);
            }
            if(wt->tab[i].hash == NULL){
                wt->tab[i].hash = saveWorkTree(wt2,absPath);
                wt->tab[i].mode = getChmod(absPath);
            }
            else{
                free(wt->tab[i].hash);
                wt->tab[i].hash = saveWorkTree(wt2,absPath);
                wt->tab[i].mode = getChmod(absPath);
            }
            FreeList(L);
            freeWorkTree(wt2);
        }
        free(absPath);
    }
    //On retourne le hash du WorkTree, tout en créant un instantané de celui-ci
    return blobWorkTree(wt);
}


int isWorkTree(char* hash){ //Vérifie si le hash en parmètre correspond au hash d'un WorkTree
    if (hash == NULL){ //Teste si le paramètre est NULL
        printf("Le hash en paramètre est NULL -> isWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    char * path = hashToPath(hash);

    if (file_exists(path)){
        free(path);
        return 0; 
    }

    strcat(path,".t");

    if (file_exists(path)){ 
        free(path);
        return 1;
    }
    free(path);
    return -1; 
}

void restoreWorkTree(WorkTree * wt, char * path){ //Restore les fichiers tels qu'ils étaient dans wt
    if (wt == NULL){ //Teste si le WorkTree est NULL
        printf("Le WorkTree est NULL -> restoreWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    if (path == NULL){ //Teste si le path est NULL
        printf("Le path est NULL -> restoreWorkTree\n");
        exit(EXIT_FAILURE) ;
    }

    for(int i = 0 ; i < wt->n ; i ++){ //On parcourt les WorkFile de wt

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

        if (statut_WT == 0){ //Si il s'agit d'un WorkFile
            
            //On copie le fichier et on met son mode au mode du fichier d'origine
            cp(absPath,copyPath);
            setMode(wt->tab[i].mode,absPath);
        }
        if (statut_WT == 1){ //Si il s'agit d'un WorkTree
            
            //On récupère le WorkTree associé et on le restore, puis on met le mode du fichier
            copyPath = realloc(copyPath,strlen(copyPath)+3);
            strcat(copyPath,".t");
            strcat(copyPath,"\0");
            WorkTree * newwt = ftwt(copyPath);
            restoreWorkTree(newwt,absPath);
            setMode(getChmod(copyPath),absPath);
            freeWorkTree(newwt);
        }
        free(absPath);
        free(copyPath);
    }
}