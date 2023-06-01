#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>

char* Affiche_inode (struct stat * Infos){
    /*info pointe vers une struct stat */
    char* type;
    int taille = Infos->st_size;
    char* date = ctime(&Infos->st_mtime);
    if (S_ISREG(Infos->st_mode)!=0){
        type="fichier ordinaire";
    }
    if (S_ISDIR(Infos->st_mode)!=0){
        type="répertoire";
    }
    if (S_ISFIFO(Infos->st_mode)!=0){
        type="fichier spécial tube(fifo)";
    }
    if (S_ISCHR(Infos->st_mode)!=0){
        type="périphérique caractère";
    }
    if (S_ISBLK(Infos->st_mode)!=0){
        type="périphérique bloc";
    }
    if (S_ISLNK(Infos->st_mode)!=0){
        type="lien symbolique";
    }
    if (S_ISSOCK(Infos->st_mode)!=0){
        type="socket(connexion réseau)";
    }
    struct passwd *pw = getpwuid(Infos->st_uid);
    printf(" %10s %10d octets, %10s modifié le: %10s\n",type,taille,pw->pw_name,date);
    return type;
}

/*ex3 : ouvrir le répertoire, puis faire comme d'habitude : faire lstat de chaque fichier avec ses test, puis faire l'affichage avec afficher_inode*/
/* voir cours 4 pour l'ouverture d'un répertoire*/

int main(int argc, char const *argv[])
{
    if(argc<2){
        fprintf(stderr,"Usage : pas assez d'argument\n");
        exit(1);
    }

    struct stat Infos;

    if (argc==2){
      DIR* rep;
      if ((rep=opendir(argv[1]))==NULL){ /* Pas besoin de vérifier si c'est un répertoire avant, car opendir renverra une erreur directement si s'en est pas un */
        perror(argv[1]);
        exit(3);
      }

      struct dirent* lect;
      char* fichier;

      while ((lect=readdir(rep))!=NULL){
        fichier =lect->d_name;
        if (lstat(fichier,&Infos)<0){
          perror(fichier);
          exit(2);
        }

        printf("%-30s",fichier);
        Affiche_inode(&Infos);
      }
      if (closedir(rep)==-1){
        perror("erreur lors de la fermeture du répertoire");
        exit(4);
      }

      exit(0);
    }
    printf("plus de 2 arguments\n");

    for (int i=1;i<argc;i++){
        if (lstat(argv[i],&Infos)<0){
            perror(argv[i]);
            exit(2);
        }
    printf("%-30s",argv[i]);
    Affiche_inode(&Infos);
    }
    return 0;
}