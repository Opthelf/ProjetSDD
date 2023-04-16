#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include "List.h"
#include "WorkTree.h"
#include "branch.h"
#include "Commit.h"
#include "Gestion_file.h"
#include "hash.h"
#include "merge.h"

//Teste si le fichier existe
int file_exists (char *file){ 
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

//Renvoie trois valeurs différentes selon si le fichier existe, est un répertoire ou bien simplement un fichier
int isFile(const char *path){
    struct stat path_stat;

    //Si le fichier existe
    if (file_exists((char*)path) == 1){
        stat(path,&path_stat); 

        //Renvoie 1 si fichier 0 si répertoire
        return S_ISREG(path_stat.st_mode);   
    }

    //Renvoie -1 si n'existe pas 
    return -1;
}

//Transforme un nombre en octal vers un nombre en décimal, pour permettre la communication entre setMode et getChmod
int octalVersDecimal(int octal){
    int decimal = 0;
    int i = 0;
    while(i*64 <= octal){
        i++;
    }
    octal = octal%64;
    decimal += 100*(i-1);
    i = 0;
    while(i*8 <= octal){
        i++;
    }
    octal = octal%8;
    decimal += 10*(i-1);
    i = 0;
    while(i <= octal){
        i++;
    }
    decimal += i-1;
    return decimal;
}

//Récupère le mode d'accès du fichier passé en paramètre
int getChmod(const char * path){
    struct stat ret;

    //Si le fichier n'existe pas
    if(stat(path,&ret)==-1){
        printf("Le fichier %s n'existe pas -> getChmod\n",path);
        exit(EXIT_FAILURE);
    }

    //récupère les droits en octal
    int octal = (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);

    //récupère les droits en décimal
    int res = octalVersDecimal(octal);

    return res;
}

//Applique le mode d'accès en paramètre au fichier en paramètre
void setMode(int mode, char * path){

    //Si le mode n'a pas de sens
    if (mode < 0 || mode > 777){
        printf("Mode %d non valable\n",mode);
        exit(EXIT_FAILURE);
    }

    //Applique le mode en paramètre au fichier
    char buff[100];
    sprintf(buff,"chmod %d %s",mode,path);
    system(buff);
}

//La fonction renvoie la liste des fichiers et dossiers dans l'endroit situé à root_dir
List * listdir(char * root_dir){

	DIR *dp;
	struct dirent *ep;
	List * L = initList();
	Cell * temp_cell;
	dp = opendir(root_dir);

    //Si il n'y a pas eu de problèmes lors de l'ouverture de root_dir
	if(dp != NULL){

        //On parcout les fichiers
		while((ep = readdir(dp)) != NULL){
			temp_cell = buildCell(ep->d_name);
			insertFirst(L,temp_cell);
		}

		(void)closedir(dp);
	}
	else{
		perror("On a pas pu ouvrir le repertoire\n");
		return NULL;
	}
	return L;
}

//Copie le contenu du fichier from dans le fichier to
void cp(char* to, char* from){

    //On récupère l'état du fichier
    int etat = isFile(from);

    //Si le fichier qu'on veut copier n'exitse pas
    if (etat == -1){
        printf("Le fichier entré en paramètre n'existe pas -> fichier non copié !(cp)\n");
        exit(EXIT_FAILURE);
    }

    //Si le fichier qu'on veut copier est un répertoire
    if (etat == 0){
        printf("Le fichier entré en paramètre est un répertoire -> on ne peut pas copier un répertoire !(cp)\n");
        exit(EXIT_FAILURE);
    }

    //On ouvre les fichiers
    FILE * f_depart = fopen(from,"r");
    FILE * f_arrivee = fopen(to,"w");

    //Si il y a un problème d'ouverture des fichiers
    if (f_depart == NULL || f_arrivee == NULL){
        printf("Erreur d'ouverture lors des fichiers %s et %s -> cp\n",to,from);
        exit(EXIT_FAILURE);
    }

    char buff[256];

    //On récupère le fichier ligne par ligne et on le met dans le fichier de destination
    while(fgets(buff,256,f_depart) != NULL){
        fprintf(f_arrivee,"%s",buff);
    }    

    //On ferme les fichiers
    fclose(f_depart);
    fclose(f_arrivee);
}

//Crée une copie du fichier au path du hash de ce fichier (avec hashToPath)
void blobFile(char* file){

    //On récupère l'état du fichier
    int etat_file = isFile(file);

    //Si le fichier est un directory
    if (etat_file == 0){
        printf("%s est un directory, on ne peut pas en créer une copie -> blobfile\n",file);
        exit(EXIT_FAILURE);
    }

    //Si le fichier n'existe pas
    if (etat_file == -1){
        printf("Le fichier %s n'existe pas -> blobfile\n",file);
        exit(EXIT_FAILURE);
    }

    //Récupère le hash du fichier
	char * hash = sha256file(file);

    //S'occupe de créer le répertoire si besoin
	char * ch = hashToFile(hash);

    //Copie le fichier file dans ch
	cp(ch,file);

    //On libère la mémoire alloué
	free(ch);
	free(hash);
}

//Concatène les deux path entre eux, en mettant entre le premier et le deuxième un '\'
char * concat_paths(char * path1,char * path2){

    //Si l'un des path est NULL
    if(path1 == NULL || path2 == NULL){
        printf("L'un des deux path, voir les deux sont NULL -> concat_paths\n");
        exit(EXIT_FAILURE);
    }

    //On récupère la taille de la chaine à allouer
    int len = strlen(path1)+ strlen(path2)+2;

    char * result = malloc(len*sizeof(char));

    //Si il y a eu une erreur d'allocation de mémoire
    if (result == NULL){
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    char * cp1 = strdup(path1);
    char * cp2 = strdup(path2);
    strcpy(result,"");


    //On récupère la path concaténer
    strcat(result,cp1);
    strcat(result,"/");
    strcat(result,cp2);
    strcat(result,"\0");

    //On libère la mémoire alloué
    free(cp1);
    free(cp2);

    return result;
}