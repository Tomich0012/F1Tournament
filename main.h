#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

//Structure d'une voiture. Sera manipulée par les processus fils.
struct Voiture{
  int numero; //Numéro de la voiture
  int pit; //Nombre de pit stop
  int out; //Booléen qui indique si la voiture est out ou non
  double best[4]; //Meilleur temps de la voiture
  double nbTour; //Nombre de tour effectué par la voiture
  double sections[3];  //Temps de chaque section
  double tourActuel; //Temps du tour actuel
};

//Structure de la mémoire partagée.
struct MemoirePartagee{
  struct Voiture tableauV[20]; //Tableau de voitures de 20 voitures 
  struct Voiture courseFinale[20]; //Tableau de voitures pour la course finale
  int nbVoituresFini; //Nombre de voitures qui ont fini la course
  int nbTourAFaire; //Nombre de tour à faire
};

// Process pour un tour
//renvoie le temps du tour
double calculTour(struct Voiture *v);

//Fonction qui gère la course Finale
void courseFinale(struct MemoirePartagee *tabVoitures, int i);

//Fonction qui gère les essais et les qualifications
void essaisEtQualifications(struct MemoirePartagee *tabVoitures, int i, double tempsTotal);

//Fonction qui permet d'afficher ce qu'il se passe
void afficheTab(struct MemoirePartagee *tabVoitures, int choix);

//Fonction qui permet de trier le tableau des voitures après chaque qualification
void triQualifications(struct MemoirePartagee *tabVoitures, int choix);

//Fonction qui gère l'affichage, les tris et la création des fichiers
void fonctionPere(struct MemoirePartagee *tabVoitures, int choix);
