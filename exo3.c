#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdbool.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"

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

int getChmod(const char * path){ //Récupère le mode d'accès du fichier passé en paramètre
    struct stat ret;
    if(stat(path,&ret)==-1){
        printf("Le fichier %s n'existe pas -> getChmod\n",path);
        exit(EXIT_FAILURE);
    }
    int octal = (ret.st_mode & S_IRUSR)|(ret.st_mode & S_IWUSR)|(ret.st_mode & S_IXUSR)|(ret.st_mode & S_IRGRP)|(ret.st_mode & S_IWGRP)|(ret.st_mode & S_IXGRP)|(ret.st_mode & S_IROTH)|(ret.st_mode & S_IWOTH)|(ret.st_mode & S_IXOTH);
    int res = octalVersDecimal(octal);
    return res;
}

void setMode(int mode, char * path){ //Applique le mode d'accès en paramètre au fichier en paramètre
    if (mode < 0 || mode > 777){ //On regarde si le mode a du sens 
        printf("Mode %d non valable\n",mode);
        exit(EXIT_FAILURE);
    }
    char buff[100];
    sprintf(buff,"chmod %d %s",mode,path);
    system(buff);
}

char * hashToPath(char *hash){ //Renvoie le hash sous forme de path (un / entre le deuxième et le troisème caractère)
    if (hash == NULL){ //On vérifie que le hash n'est pas NULL, auquel cas il y a eu un souci avant donc on exit complètement
        printf("Le hash est NULL -> hashToPath\n");
        exit(EXIT_FAILURE);
    }

	int l = strlen(hash);
	char * path = malloc(sizeof(char)*(l+10));
    strcpy(path,"");
	path[0] = hash[0];
	path[1] = hash[1];
	path[2] = '/';
	
	int i;
	for( i = 3;i<=l;i++){
		path[i] = hash[i-1];
	}
	path[i]='\0';
	return path;
}

char * hashToFile(char *hash){ //Crée le directory avec les deux premiers caractères du hash, puis retourne le path
    char * ch2 = strdup(hash);
    ch2[2]='\0'; //On dit que le chaine s'arrête là
    struct stat st={0};
    if(stat(ch2,&st) == -1){ //Si le directory n'existe pas déjà, on le crée avec tout les droits d'utilisateurs et aucuns pour les autres
        mkdir(ch2,0700);
    }
    free(ch2);
    return hashToPath(hash);
}

List * listdir(char * root_dir){
	DIR *dp;
	struct dirent *ep;
	List * L = initList();
	Cell * temp_cell;
	dp = opendir(root_dir);
	if(dp != NULL){
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

struct stat st = {0};


void cp(char* to, char* from){
    int etat = isFile(from);
    if (etat == -1){ //Si le fichier qu'on veut copier n'existe pas
        printf("Le fichier entré en paramètre n'existe pas -> fichier non copié !(cp)\n");
        exit(EXIT_FAILURE);
    }

    if (etat == 0){ //Si le fichier qu'on veut copier est un répertoire
        printf("Le fichier entré en paramètre est un répertoire -> on ne peut pas copier un répertoire !(cp)\n");
        exit(EXIT_FAILURE);
    }

    FILE * f_depart = fopen(from,"r");
    FILE * f_arrivee = fopen(to,"w");
    if (f_depart == NULL || f_arrivee == NULL){ //Si il y a un problème d'ouverture des fichiers
        printf("Erreur d'ouverture lors des fichiers %s et %s -> cp\n",to,from);
        exit(EXIT_FAILURE);
    }

    char buff[256];
    while(fgets(buff,256,f_depart) != NULL){ //On récupère le fichier ligne par ligne et on le met dans le fichier de destination
        fprintf(f_arrivee,"%s",buff);
    }    

    fclose(f_depart);
    fclose(f_arrivee);
}

void blobFile(char* file){ //Crée une copie du fichier au path du hash de ce fichier (avec hashToPath)
    int etat_file = isFile(file);
    if (etat_file == 0){ //Si le fichier est un directory
        printf("%s est un directory, on ne peut pas en créer une copie -> blobfile\n",file);
        exit(EXIT_FAILURE);
    }
    if (etat_file == -1){ //Si le fichier n'existe pas
        printf("Le fichier %s n'existe pas -> blobfile\n",file);
        exit(EXIT_FAILURE);
    }

	char * hash = sha256file(file); //Récupère le hash du fichier
	char * ch = hashToFile(hash); //S'occupe de créer le répertoire si besoin
	cp(ch,file); //Copie le fichier file dans ch

	free(ch);
	free(hash);
}