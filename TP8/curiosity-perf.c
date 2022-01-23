#include "environnement.h"
#include "programme.h"
#include "interprete.h"
#include "generation_terrains.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define atoa(x) #x

#define NB_SORTIES 0
#define NB_COULE 1
#define NB_ECRASE 2
#define NB_BLOQUE 3
#define NB_PAS_MOYEN 4

void gestion_erreur_terrain(erreur_terrain e)
{
    switch (e)
    {
    case OK:
        break;

    case ERREUR_FICHIER:
        printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
        exit(1);

    case ERREUR_LECTURE_LARGEUR:
        printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
        exit(1);

    case ERREUR_LECTURE_HAUTEUR:
        printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
        exit(1);

    case ERREUR_LARGEUR_INCORRECTE:
        printf("Erreur lecture du terrain : largeur incorrecte\n");
        exit(1);

    case ERREUR_HAUTEUR_INCORRECTE:
        printf("Erreur lecture du terrain : hauteur incorrecte\n");
        exit(1);

    case ERREUR_CARACTERE_INCORRECT:
        printf("Erreur lecture du terrain : caractère incorrect\n");
        exit(1);

    case ERREUR_LIGNE_TROP_LONGUE:
        printf("Erreur lecture du terrain : ligne trop longue\n");
        exit(1);

    case ERREUR_LIGNE_TROP_COURTE:
        printf("Erreur lecture du terrain : ligne trop courte\n");
        exit(1);

    case ERREUR_LIGNES_MANQUANTES:
        printf("Erreur lecture du terrain : lignes manquantes\n");
        exit(1);

    case ERREUR_POSITION_ROBOT_MANQUANTE:
        printf("Erreur lecture du terrain : position initiale du robot manquante\n");
        exit(1);

    default:
        printf("ERREUR NON GEREE\n");
        break;
    }
}

void affichage_position_programme(erreur_programme e)
{

    int i;

    printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);

    printf("%s\n", e.ligne);

    /* Impression de e.num_colonne-1 espaces */

    for (i = 1; i < e.num_colonne; i++)
    {

        printf(" ");
    }
    /* Impression d'un curseur de position */
    printf("^\n");
}

void init_env_avec_terrain(Terrain T, int l, int h, Environnement *envt)
{
    envt->t = T;
    init_robot(&(envt->r), (int)l / 2, (int)h / 2, Est);
}

void gestion_erreur_programme(erreur_programme e)
{

    switch (e.type_err)
    {

    case OK_PROGRAMME:
        break;

    case ERREUR_FICHIER_PROGRAMME:
        printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
        exit(2);
    case ERREUR_BLOC_NON_FERME:
        printf("Erreur lecture du programme : bloc non fermé\n");
        exit(2);
    case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
        printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
        affichage_position_programme(e);
        exit(2);
    case ERREUR_COMMANDE_INCORRECTE:
        printf("Erreur lecture du programme : commande incorrecte\n");
        affichage_position_programme(e);
        exit(2);
    }
}

resultat_inter simuler_programme_sur_terrain(Programme prog, Terrain Terrains_Valides, int x0, int y0, int nbstepmax, int *nb_step_effect, Environnement *envt)
{
    /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
    etat_inter etat;
    resultat_inter res;
    int nbstep = 0;

    /* Initialisation de l'état */
    init_etat(&etat);
    res = OK_ROBOT;

    do
    {
        res = exec_pas(&prog, envt, &etat);
        nbstep++;
    } while (res == OK_ROBOT && nbstep < nbstepmax);

    *nb_step_effect = nbstep;

    return res;
}

void ecrire_terrains(FILE *f, Terrain Terrains_Valides[], int nb_terrains_valides, int l, int h, Environnement envt[])
{

    int i;
    for (i = 0; i < nb_terrains_valides; i++)
    {
        // Afficher le premier environnement
        ecrire_envt(f, &(envt[i]));
        fprintf(f, " *******************************************\n\n");
    }
}

/* Afficher le terrain avec la position et l'orientation du robot */
void ecrire_envt(FILE *f, Environnement *envt)
{
    int i, j;
    int h, l;
    char c;

    h = hauteur(envt->t);
    l = largeur(envt->t);

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < l; i++)
        {
            if ((i == abscisse(envt->r)) && (j == ordonnee(envt->r)))
            {
                // Afficher le robot selon son orientation
                switch (orient(envt->r))
                {
                case Nord:
                    c = '^';
                    break;
                case Est:
                    c = '>';
                    break;
                case Sud:
                    c = 'v';
                    break;
                case Ouest:
                    c = '<';
                    break;
                }
            }
            else
            {
                // Afficher la case
                switch ((envt->t).tab[i][j])
                {
                case LIBRE:
                    c = '.';
                    break;
                case ROCHER:
                    c = '#';
                    break;
                case EAU:
                    c = '~';
                    break;
                }
            }
            fprintf(f, "%c", c);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

int main(int argc, char **argv)
{
    Programme prog;
    FILE *resFile;
    Terrain terrain_temp;
    resultat_inter res;
    erreur_programme errp;
    int N, l, h, graine, nb_step_max, i, nb_terrains_valides = 0, index = 0, nb_step = 0;
    int nb_sortie, nb_bloquee, nb_obstacle, nb_pas_sortie;
    float dObst;

    if (argc < 9)
    {
        printf("Usage: %s <fichier_programme> <N> <L> <H> <dObst> <graine> <nb_step_max> <fichier_resultat> \n", argv[0]);
        return 1;
    }
    //
    // Recuperation des parametres
    //
    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    graine = strtol(argv[6], NULL, 10);
    nb_step_max = strtol(argv[7], NULL, 10);
    dObst = strtof(argv[5], NULL);
    // Definiton
    Terrain Terrains_Valides[N];
    Environnement envt[N];

    // Initialiser le generateur aleatoire par la valeur de la graine
    srand(graine);

    // test de l et h
    if (l > DIM_MAX || l % 2 == 0)
    {
        printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if (h > DIM_MAX || h % 2 == 0)
    {
        printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if ((dObst > 1) || (dObst < 0))
    {
        printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
        return 1;
    }

    // Ouvrir le fichier de resultats
    resFile = fopen(argv[8], "w");

    /* Lecture du programme */
    errp = lire_programme(&prog, argv[1]);
    gestion_erreur_programme(errp);

    //Generer aleatoirement N terrains avec les caracteristiques : <L> <H> <dObst> <graine>:
    //construire le tableau des terrains aleatoires valides dont la taille effective sera nb_terrains_valides
    for (i = 0; i < N; i++)
    {
        generation_aleatoire(&terrain_temp, l, h, dObst, graine);

        // si le terrain est valide alors le rajoute dans notre tableau
        if (terrain_temp.hauteur == h && terrain_temp.largeur == l)
        {
            nb_terrains_valides++;
            Terrains_Valides[index] = terrain_temp;
            init_env_avec_terrain(Terrains_Valides[i], l, h, &(envt[i]));
            ;
            index = index + 1;
        }
    }

    ecrire_terrains(resFile, Terrains_Valides, nb_terrains_valides, l, h, envt);
    // Etape 1: [ligne 1] : n (nombre de terrains testés)
    fprintf(resFile, "%d\n", nb_terrains_valides);

    // Etape 2: simuler notre programme sur le tableau de terrains
    //     [ligne i+1] (pour le terrain numéro i) :
    // si curiosity est sorti, le nombre de pas effectués
    // sinon, curiosity est «bloqué» (-1), «tombé dans l'eau» (-2), «écrasé contre un rocher» (-3)

    nb_sortie = 0 ;
    nb_bloquee = 0 ;
    nb_obstacle = 0 ;
    nb_pas_sortie = 0;

    for (i = 0; i < nb_terrains_valides; i++)
    {
        res = simuler_programme_sur_terrain(prog, Terrains_Valides[i], (int)l / 2, (int)h / 2, nb_step_max, &nb_step, &(envt[i]));

        switch (res)
        {
        case SORTIE_ROBOT:
            fprintf(resFile, "%d\n", nb_step);
            nb_sortie ++;
            nb_pas_sortie = nb_pas_sortie + nb_step;
            break;
        case ARRET_ROBOT:
            fprintf(resFile, "%d\n", -1);
            nb_bloquee ++;
            break;
        case PLOUF_ROBOT:
            fprintf(resFile, "%d\n", -2);
            break;
        case CRASH_ROBOT:
            fprintf(resFile, "%d\n", -3);
            nb_obstacle ++;
            break;
        default:
            break;
        }
        //fprintf(resFile, " *******************************************\n\n");
    }

    // Etap3:
    // le programme affiche(sur la sortie standard) des statistiques pertinentes de «performance» du programme :
    // nombre et pourcentage de terrains d'où le robot est sorti/où il est resté bloqué/où il est rentré dans un obstacle ;
    //  nombre moyen de pas effectués pour les sorties.
    printf("Nombre de terrains d'où le robot est sorti est : %d et le pourcentage est : %f \%\n", nb_sortie, 100*((float)nb_sortie/nb_terrains_valides)) ;
    printf("Nombre de terrains d'où le robot est bloque est : %d et le pourcentage est : %f \%\n", nb_bloquee, 100*((float)nb_bloquee/nb_terrains_valides)) ;
    printf("Nombre de terrains d'où le robot est rentré dans un obstacle  est : %d et le pourcentage est : %f \%\n", nb_obstacle, 100*((float)nb_obstacle/nb_terrains_valides)) ;
    if(nb_sortie != 0){
        printf("Le nombre moyen de pas effectues pour les sorties est : %f \n", ((float)nb_pas_sortie/nb_sortie));
    }
    else{
        printf("Le nombre moyen de pas effectues pour les sortie est 0 \n");
    }
    
    ecrire_terrains(resFile, Terrains_Valides, nb_terrains_valides, l, h, envt);

    // Fermer le fichier de resultats
    fclose(resFile);
}