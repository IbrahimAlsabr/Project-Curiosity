#include "environnement.h"
#include "programme.h"
#include "interprete.h"

#include <stdio.h> 
#include <stdlib.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {
  case OK: break;
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
  }
}


void affichage_position_programme(erreur_programme e) {
     
	 int i;
     
	 printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
     
	 printf("%s\n", e.ligne);
     
	 /* Impression de e.num_colonne-1 espaces */
     
	 for(i = 1; i < e.num_colonne; i++) {
     
     	 printf(" ");
     
    	 }
     /* Impression d'un curseur de position */
     printf("^\n");

}

void gestion_erreur_programme(erreur_programme e) {
     
	 switch(e.type_err) {
  
           case OK_PROGRAMME: break;
  
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


void lire_fichier(FILE *fichier , char *nom_fichier , char *nom_terrain , char *nom_prog , char *etat_final , char *orientation_final , int *max_pas , int *x , int *y){
     
      fichier = fopen(nom_fichier,"r");
      
      if(fichier == NULL){
      
        printf("Erreur : Fichier %s Vide !!! \n",nom_fichier);
        
        }
        
        fscanf(fichier,"%s",nom_terrain);
        
        fscanf(fichier,"%s",nom_prog);
        
        fscanf(fichier,"%d",max_pas);
        
        fscanf(fichier," %c",etat_final);
        
        fscanf(fichier,"%d %d",y,x);
        
        fscanf(fichier," %c",orientation_final);                /*Erreur fichier de test non traite */ 
        
}        
    
char resultat_inter_to_char(resultat_inter res){

      switch(res){
      
            case(OK_ROBOT): return 'N';break;  
            
            case(SORTIE_ROBOT): return 'S';break;
            
            case(ARRET_ROBOT): return 'F';break;  
            
            case(PLOUF_ROBOT): return 'P';break;
            
            case(CRASH_ROBOT): return 'O';break;
            
            case(ERREUR_PILE_VIDE): return 'O';break;
            
            case(ERREUR_ADRESSAGE): return 'O';break;
               
            case(ERREUR_DIVISION_PAR_ZERO): return 'O';break;
      
            } 
         
       return 'O';  
}               

Orientation   Caractere_to_Orientation(char c){   

              switch(c){
                       
                        case 'N' : return Nord;
                                break;
                                
                        case 'E' : return Est;
                                break;
                                
                        case 'S' : return Sud;
                                break;
                                
                        case 'O' : return Ouest;
                                break;
                        
                        default: printf("Erreur : Orientation non conforme\n");
                         
                         }

}



int main (int argc ,char *argv[]){

     FILE *fichier ; 
     
     char nom_terrain[30] , nom_prog[30] , etat_final , orientation_final ;   //erreur fichier interpreteur.c 
     
     int max_pas = -1 , x = -1 , y = -1 , compteur = 0;           //x et y etant les coordonnes de la position finale du robot 
 
      Environnement envt;
  
      Programme prog_test;
  
      erreur_terrain errt;
  
      erreur_programme errp;
  
      etat_inter etat;
  
      resultat_inter res;
  
     
     if (argc != 2) {                            
     
	      printf("Usage: %s <fichier>\n", argv[0]);
             
        return -1;     
    
	      }             
       
      lire_fichier(fichier , argv[1]  , nom_terrain , nom_prog , &etat_final , &orientation_final , &max_pas , &x , &y);            
   
      errt = initialise_environnement(&envt, nom_terrain);
     
      gestion_erreur_terrain(errt);
     
     /* Lecture du programme */
    
      errp = lire_programme(&prog_test, nom_prog);
     
      gestion_erreur_programme(errp);

     /* Initialisation de l'état */
     
      init_etat(&etat);
    
	    while(compteur < max_pas){
      
           afficher_envt(&envt);  
      	   
           res = exec_pas(&prog_test, &envt, &etat);
           
           compteur ++;
           
           }             
           
       if((resultat_inter_to_char(res) == etat_final) && envt.r.o ==  Caractere_to_Orientation(orientation_final) && (envt.r.x == x) && (envt.r.y == y))  
       
         printf("Test Reussi !!!\n");  
       
         else printf("Test Fail \n"); 
                  
         printf(" %d %d  %c %c \n",envt.r.x,envt.r.y,resultat_inter_to_char(res),envt.r.o); 
       
       return 1;           
             
}             
