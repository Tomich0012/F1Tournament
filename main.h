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
struct Car{
  int numero; //Numéro de la voiture
  int pit; //Nombre de pit stop
  int out; //Booléen qui indique si la voiture est out ou non
  double best[4]; //Meilleur temps de la voiture
  double tourNbr; //Nombre de tour effectué par la voiture
  double sections[3];  //Temps de chaque section
  double currentLap; //Temps du tour actuel
};

//Structure de la mémoire partagée.
struct SharedMemory{
  struct Car vTab[20]; //Tableau de voitures de 20 voitures 
  struct Car finalRace[20]; //Tableau de voitures pour la course finale
  int nbrCarFinished; //Nombre de voitures qui ont fini la course
  int lapNbrToDo; //Nombre de tour à faire
  sem_t semaphore; //Sémaphore
};

// Process pour un tour
//renvoie le temps du tour
double lapCalc(struct Car *v);

//Fonction qui gère la course Finale
void finalRace(struct SharedMemory *carsTab, int i);

//Fonction qui gère les essais et les qualifications
void fpAndQualif(struct SharedMemory *carsTab, int i, double totalTime);

//Fonction qui permet d'afficher ce qu'il se passe
void tabDisplay(struct SharedMemory *carsTab, int pick);

//Fonction qui permet de trier le tableau des voitures après chaque qualification
void qualifSort(struct SharedMemory *carsTab, int pick);

//Fonction qui gère l'affichage, les tris et la création des fichiers
void daddyFunction(struct SharedMemory *carsTab, int pick);
