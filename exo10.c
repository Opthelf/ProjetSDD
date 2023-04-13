#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "exo1.h"
#include "exo2.h"
#include "exo3.h"
#include "exo4.h"
#include "exo5.h"
#include "exo6.h"
#include "exo7.h"
#include "exo8.h"
#include "exo9.h"
#include "exo10.h"

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