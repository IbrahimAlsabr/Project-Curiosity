#include "terrain.h"
#include <stdio.h>

void gestion_erreur_terrain(erreur_terrain l) {
 switch(l){
    case ERREUR_LECTURE_LARGEUR    : printf("Erreur Lecture de la largeur!\n")                                    ;break;
    case ERREUR_LARGEUR_INCORRECTE : printf("Erreur:La largeur negative ou superieur a DIM_MAX !\n")              ;break;
    case ERREUR_LECTURE_HAUTEUR    : printf("Erreur:La lecture de la hauteur!\n")                                 ;break;
    case ERREUR_HAUTEUR_INCORRECTE : printf("Erreur:La hauteur est negative ou superieur a DIM_MAX !\n")          ;break;
    case ERREUR_LIGNES_MANQUANTES  : printf("Erreur:Ligne manquante !\n")                                         ;break;
    case ERREUR_LIGNE_TROP_COURTE  : printf("Erreur:Ligne est trop courte !\n")                                   ;break;
    case ERREUR_LIGNE_TROP_LONGUE  : printf("Erreur:Ligne est trop longue !\n")                                   ;break;
    case ERREUR_CARACTERE_INCORRECT: printf("Erreur:Caractere incorrect !\n")                                     ;break;
    case ERREUR_POSITION_ROBOT_MANQUANTE: printf("Erreur:Robot absent de la carte !\n")                           ;break;
    default:;
    }
}


int main(int argc, char ** argv) {
  Terrain t;
  int x, y;
  
  if(argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  while(lire_terrain(argv[1], &t, &x, &y) == ERREUR_FICHIER){
  	printf("Ouverture du fichier impossible, entrez un autre fichier : \n");
  	scanf("%s", argv[1]);
  }	
  
  gestion_erreur_terrain(lire_terrain(argv[1], &t, &x, &y));
  lire_terrain(argv[1], &t, &x, &y);
  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
}

