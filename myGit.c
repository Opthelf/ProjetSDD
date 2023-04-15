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
#include "exo11.h"

//Commandes git
int main(int argc,char * argv[]){

    if (argc == 1){
        printf("Voici une liste des commandes que vous pouvez réaliser avec ./myGit ->\n\n");
        affiche_commande();
        return 0;
    }

    //Si la commande est init
    if (strcmp(argv[1],"init") == 0){
        initRefs();
        initBranch();
        return 0;
    }

    //Si la commande demande d'afficher toutes les branches
    if (strcmp(argv[1],"list-refs") == 0){

        //Si le fichier .refs n'est pas initialisé
        if (file_exists(".refs") == 0){
            printf("Aucun répertoire\n");
        }

        //Sinon on les affiche
        else{
            printf("Refs :\n");
            List * L = listdir(".refs");
            Cell * temp = *L;

            //On parcours la liste des branches
            while(temp != NULL){

                //Si ce sont bien des fichiers désirés on les affiche
                if ((strcmp(temp->data,".")) != 0 && (strcmp(temp->data,"..") != 0)){
                    printf("\t%s\n",temp->data);
                }

                temp = temp->next;
            }
            FreeList(L);
        }
        return 0;
    }

    //Si la commande demande de créer une référence
    if (strcmp(argv[1],"create-ref") == 0){

        //Si il n'y a pas assez d'arguments
        if (argc != 4){
            printf("Pas assez d'argument pour créer un ref !\n");
            return 0;
        }
        
        //On fait l'update
        createUpdateRef(argv[2],argv[3]);
        return 0;
    }

    //Si la commande demande d'effacer une branche
    if (strcmp(argv[1],"delete-ref") == 0){

        //Si il n'y a pas assez d'arguments
        if (argc != 3){
            printf("Pas assez d'argument pour delete un ref !\n");
            return 0;
        }

        //On efface la branche
        deleteRef(argv[2]);
        return 0;
    }

    //Si la commande demande d'ajouter des fichiers à commit prochainement
    if (strcmp(argv[1],"add") == 0){

        //Si il n'y a pas assez d'arguments
        if (argc < 3){
            printf("Pas assez d'argument pour add !\n");
            return 0;
        }

        //On ajoute le nombre de fichiers passés en arguments dans le fichier .add
        int i = 2;
        while(i < argc){
            myGitAdd(argv[i]);
            i++;
        }
        return 0;
    }

    //Si la commande demande de lister les fichiers ajoutés à commit prochainement
    if (strcmp(argv[1],"list-add") == 0){

        //Si le nombre d'arguments n'est pas suffisant
        if(argc != 2){
            printf("Nombre d'arguments incorrects pour list-add\n");
            return 0;
        }

        //Si le fichier .add n'existe pas
        if (file_exists(".add") == 0){
            printf("Le fichier .add n'a pas été initialisé\n");
            return 0;
        }

        //Si le fichier .add existe
        else{

            WorkTree * WT = ftwt(".add");

            //Si le WorkTree est NULL, donc que .add est vide
            if (WT == NULL){
                printf("Aucun fichier n'a été add");
            }

            //On récupère la chaine du WorkTree et on l'affiche
            char * str = wtts(WT);
            printf("Added :\n%s\n",str);

            freeWorkTree(WT); 
            free(str);
        }
        return 0;
    }

    //Si la commande demande de supprimer les fichiers ajoutés dans .add
    if (strcmp(argv[1],"clear-add") == 0){

        //Si le nombre d'arguments est incorrect
        if(argc != 2){
            printf("Nombre d'arguments incorrects pour clear-add\n");
            return 0;
        }

        //Supprime le fichier .add et tout son contenu
        system("rm .add");
        return 0;
    }

    //Si la commande demande de commit une branche
    if (strcmp(argv[1],"commit") == 0){

        //Si le nombre d'arguments est incorrect
        if ((argc < 3) || (argc == 4) || (argc > 5)){
            printf("Nombre d'arguments incorrects pour réaliser un commit\n");
            return 0;
        }

        //Si il n'y a pas de message
        if (argc == 3){
            myGitCommit(argv[2],NULL);
            return 0;
        }

        //Si il y a un message
        if (argc == 5){

            //Si il y a bien l'option -m qui précise qu'il y a un message
            if (strcmp(argv[3],"-m") == 0){
                myGitCommit(argv[2],argv[4]);
                return 0;
            }
        }
    }

    //Si l'utilisateur veut récupérer la branche courante
    if (strcmp(argv[1],"get-current-branch") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 2){
            printf("Nombre d'arguments incorrects pour afficher la branche courante\n");
            return 0;
        }

        //On la récupère puis on l'affiche
        char * current_branch = getCurrentBranch();
        printf("La branche courante est -> %s\n",current_branch);

        free(current_branch);

        return 0;
    }

    //Si l'utilisateur veut créer une nouvelle branche
    if (strcmp(argv[1],"branch") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 3){
            printf("Vous ne pouvez pas créer une branche avec ce nombre d'arguments\n");
            exit(EXIT_FAILURE);
        }

        //On récupère le nom de la branche à créer
        char * branch_name = argv[2];

        //Si la branche existe déjà
        if (branchExists(branch_name) == 1){
            printf("La branche %s existe déjà !\n",branch_name);
            exit(EXIT_FAILURE);
        }

        createBranch(branch_name);

        return 0;
    }

    //Si l'utilisateur veut afficher tout les commit d'une branche
    if (strcmp(argv[1],"print-branch") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 3){
            printf("Vous ne pouvez pas afficher la branche avec ce nombre d'arguments\n");
            exit(EXIT_FAILURE);
        }

        //On récupère le nom de la branche à afficher
        char * branch_name = argv[2];

        //Si elle n'existe pas
        if (branchExists(branch_name) == 0){
            printf("La branche %s n'existe pas !\n",branch_name);
            exit(EXIT_FAILURE);
        }

        printBranch(branch_name);
        
        return 0;
    }

    //Si l'utilisateur veut restaurer une version correspondant à une branche
    if (strcmp(argv[1],"checkout-branch") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 3){
            printf("Vous ne pouvez pas revenir sur une branche avec ce nombre d'arguments\n");
            exit(EXIT_FAILURE);
        }

        //On récupère la nom de la branche à restaurer
        char * branch_name = argv[2];

        //Si la branche n'existe pas déjà
        if (branchExists(branch_name) == 0){
            printf("La branche %s n'existe pas !\n",branch_name);
            exit(EXIT_FAILURE);
        }

        myGitCheckoutBranch(branch_name);
        
        return 0;
    }

    //Si l'utilisateur veut restaurer un commit
    if (strcmp(argv[1],"checkout-commit") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 3){
            printf("Vous ne pouvez pas revenir sur un commit avec ce nombre d'arguments\n");
            exit(EXIT_FAILURE);
        }

        //On récupère le pattern à chercher
        char * pattern = argv[2];

        //On restaure le commit
        myGitCheckoutCommit(pattern);
        
        return 0;
    }

    //Si l'utilisateur veut merge une branche avec sa branche courante
    if (strcmp(argv[1],"merge") == 0){

        //Si le nombre d'arguments est incorrect
        if (argc != 4){
            printf("Le nombre d'arguments ne permet pas de merge\n");
            exit(EXIT_FAILURE);
        }

        List * conflicts = merge(argv[2],argv[3]);

        //Si il n'y a pas eu de conflits
        if (conflicts == NULL){
            printf("La fusion s'est déroulé à merveille !\n");
            return 0;
        }

        //On récupère la branche courante
        char * current = getCurrentBranch();

        int choix;

        printf("Il y a des collisions, la fusion a été annulé.\n");
        printf("Veuillez choisir entre les options suivantes ->\n");
        printf("1-Garder la branche %s\n",current);
        printf("2-Garder la branche %s\n",argv[2]);
        printf("3-Choisir manuellement pour chaque fichier en conlit la version de quelle branche vous souhaitez\n");
        scanf("%i",&choix);

        //On sélectionne le choix de l'utilisateur
        switch (choix){

            //On récupère uniquement les fichiers sans conflits avec la branche courante
            case 1 :
                createDeletionCommit(argv[2],conflicts,argv[3]);
                merge(argv[2],argv[3]);
        
                break;

            //On récupère uniquement les fichiers sans conflits avec la branche en paramètre
            case 2 :
                myGitCheckoutBranch(argv[2]);
                createDeletionCommit(current,conflicts,argv[3]);
                merge(current,argv[3]);
                

                break;

            case 3 :
                printf("Vous allez choisir manuellement pour chaque fichier posant problème la version de quelle branche vous souhaitez garder.\n");
                printf("1 -> branche %s\n",current);
                printf("2 -> branche %s\n",argv[2]);

                List * L_current = initList();
                List * L_branch = initList();
                tri_par_choix(conflicts,&L_current,&L_branch);
                
                createDeletionCommit(current,L_branch,argv[3]);
                createDeletionCommit(argv[2],L_current,argv[3]);
                merge(argv[2],argv[3]);
                printf("Fin de la fusion par choix ! C'est un succès\n");
                FreeList(L_current);
                FreeList(L_branch);
                
                /*

                
                
                
                */
                
                break;

            default :
                printf("La valeur entrée n'est pas accepté -> abort fusion request\n");
                free(current);
                FreeList(conflicts);
                exit(EXIT_FAILURE);
        }

        //On libère la mémoire allouée
        FreeList(conflicts);
        free(current);

        return 0;
    }

    //Si l'utilisateur ne fait aucune des commandes précédentes, le programme affiche toutes les commandes possibles
    printf("Votre commande n'a pas été trouvé.\nVoici une liste des commandes que vous pouvez taper :\n\n");
    affiche_commande();

    return 0;
}