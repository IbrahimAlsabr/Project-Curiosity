#ifndef _TYPE_PILE_H_
#define _TYPE_PILE_H_

#define TAILLE_MAX 100

typedef struct {
  int n;
  int tab[TAILLE_MAX];
} PileEntiers;

/* Constructeurs */

/* Cr�er une pile vide */
void creer_pile(PileEntiers * p);

/* Op�rations d'acc�s */

/* Retourne vrai ssi p est vide */
int est_vide(PileEntiers p);

/* Renvoie l'entier en haut de la pile */
/* p doit �tre non vide */
int sommet(PileEntiers p);

/* Renvoie le nombre d'�l�ments dans la pile */
int taille(PileEntiers p);

/* Afficher les �l�ments de la pile */
void print(PileEntiers p);

/* Op�rations de modification */

/* Vider la pile p */
void vider(PileEntiers * p);

/* Empiler un entier x */
/* Pr�condition : taille(p) < TAILLE_MAX */
void empiler(PileEntiers * p, int x);

/* Supprimer l'entier en haut de la pile */
/* Pr�condition : p non vide */
void depiler(PileEntiers * p);

#endif
