#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_cases_occupees(Terrain T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T.tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, i, l, h;
  float dObst;
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  // Initialisation de la fonction random
  // A compléter

  // A compléter
  // Génération aléatoire des terrains

  float densite_obs = 0;
  float denstie_obs_total = 0;
  int nb_terrains_generes = 0;

  for(i=0;i<N;i++){
    generation_aleatoire(&T,l,h,dObst,1);
    if(T.hauteur == h && T.largeur == l){
      nb_terrains_generes ++;
      ecrire_terrain(resFile, T, (int)l/2, (int) h/2);
      densite_obs = densite_obstacles(T);
      denstie_obs_total = denstie_obs_total + densite_obs;

      if (densite_obs > dObst)
      {
        fprintf(resFile, "la densite obtenue est plus grande que la densité demandé\n");
        fprintf(resFile, "la densite obtenue est :%f, la densite attendue est : %f\n", densite_obs,dObst);
      }

      else if (densite_obs == dObst)
      {
        fprintf(resFile, "la densite obtenue est égale à la densite attendue\n");
      }
      
      else
      {
        fprintf(resFile, "la densite demandé est plus petite que la densité obtenue\n");
        fprintf(resFile, "la densite obtenue est :%f, la densite attendue est : %f\n", densite_obs,dObst);
      }
      

  }
  }

  float densite_moyenne = denstie_obs_total/nb_terrains_generes;
  float pourc_terrains_valides = (((float)nb_terrains_generes/N) * 100) ;
  printf("%f ",pourc_terrains_valides );
  fprintf(resFile, "La densite d'obstacles moyennes est de : %f\n", densite_moyenne);
  fprintf(resFile, "Le pourcentage de terrain valides par rapport au nombre total est de : %f pourcent\n", pourc_terrains_valides) ;
  // Écriture des terrains générés dans le fichier resFile
  // Écriture/Affichage des statistiques

  // fermeture des fichiers
  fclose(resFile);
  return 0;
}
