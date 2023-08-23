#include "main.h"
#include "utils.c"
#include <semaphore.h>

sem_t semaphore;

#define  ESSAI1  0
#define ESSAI2  1
#define ESSAI3  2
#define QUALIF1  3
#define QUALIF2  4
#define QUALIF3  5
#define FINALE  6
#define QUIT  10

// Process pour un lap
//renvoie le temps du lap
double lapCalc(struct Car *v){
  double timeInPit, lap = 0.0, section; //Temps du lap, temps de chaque section
  if (out() == 1) { //Si la voiture est out
    v->out = 1; //On met le booléen out à 1
  }
  else{ //Sinon
    for(int i = 0; i < 3; i++){ //On calcule le temps de chaque section
      section = sectionCalc(); //On calcule le temps de la section
      if(i == 2){ //Si c'est la dernière section
        timeInPit = stand(); //On calcule le temps du stand
        if (timeInPit > 0) { //Si le temps du stand est supérieur à 0
          v->pit += 1; //On incrémente le nombre de pit stop
          section += timeInPit; //On ajoute le temps du stand à la section
        }
      }
      lap += section; //On ajoute le temps de la section au temps du lap
      msleep(section); //On attend le temps de la section
      v->sections[i] = section; //On ajoute le temps de la section au tableau des sections
      v->tourNbr += 1; //On incrémente le nombre de lap
      v->best[i] = bestTime(section, v->best[i]); //On compare le temps de la section au meilleur temps de la section
    }
    v->best[3] = bestTime(lap, v->best[3]); //On compare le temps du lap au meilleur temps du lap
    v->currentLap= lap; //On ajoute le temps du lap à la variable currentLap
  }
  return lap; //On renvoie le temps du lap
}

void finalRace(struct SharedMemory *carsTab, int i){ //i = numéro de la voiture
  struct Car v = carsTab->carTab[i]; //On récupère la voiture
  struct Car *vPoint = &v; //On crée un pointeur sur la voiture
  carVarInit(vPoint); //On initialise les variables de la voiture


  v.tourNbr= 0; //On initialise le nombre de lap à 0

  sleep(1); //On attend 1 seconde
  
  while (v.tourNbr < carsTab->lapNbrToDo && v.out == 0) { //carsTab->lapNbrToDo 
    lapCalc(vPoint);
    sem_wait(&semaphore);
    carsTab->carTab[i] = v;
    sem_post(&semaphore); // Libération du sémaphore
    msleep(1000);
  }
  carsTab->nbrCarFinished += 1; //On incrémente le nombre de voitures qui ont fini la course
  exit(0); //On quitte le processus
}

void fpAndQualif(struct SharedMemory *carsTab, int i, double totalTime){ //i = numéro de la voiture
  struct Car v; //On crée une voiture
  struct Car *vPoint = &v; //On crée un pointeur sur la voiture
  if(totalTime >= 1080){ // soit essais soit qualif 1 
    v = car_init(i); //On initialise la voiture
  }
  else{ 
    v = carsTab->carTab[i]; //On récupère la voiture
    carVarInit(vPoint); //On initialise les variables de la voiture

  }
  double ongoingTime = 0; 
  //Boucle course
  while (ongoingTime < totalTime && v.out == 0) { //totalTime
    ongoingTime += lapCalc(vPoint); //On calcule le temps du lap
    sem_wait(&semaphore);
    carsTab->carTab[i] = v;
    sem_post(&semaphore); // Libération du sémaphore
    if (totalTime == 5400 || totalTime == 3600) { // si c'est un essaie
	sleep(1.5);
    }
    else{ 
	sleep(1.5); 
    }
  }
  carsTab->nbrCarFinished += 1; //On incrémente le nombre de voitures qui ont fini la course
  exit(0);
}

void tabDisplay(struct SharedMemory *carsTab, int pick){ 
  //pick = 0 pour essais 1, 1 pour essais 2, 2 pour essais 3, 3 pour qualif 1, 4 pour qualif 2, 5 pour qualif 3, 6 pour course final
  struct SharedMemory mem = *carsTab; //On récupère la mémoire partagée
  struct SharedMemory *memoryPoint = &mem; //On crée un pointeur sur la mémoire partagé | 

  //pas sur de ça
  int final = 0; //Booléen qui indique si c'est la final ou non
  if(pick == FINALE){  //Si c'est la final
    carSort(memoryPoint->carTab, 5); //On trie le tableau des voitures

    final = 1; //On met le booléen final à 1
  }


  else{ 
    carSort(memoryPoint->carTab, 3); //On trie le tableau des voitures
  }
  system("clear");
  switch (pick) {
    case ESSAI1: 
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             ESSAIS 1                               		         |\n");
    break;
    case ESSAI2:
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             ESSAIS 2                               		         |\n");
    break;
    case ESSAI3:
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             ESSAIS 3                               		         |\n");
    break;
    case QUALIF1:
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             QUALIFICATIONS 1                 				 |\n");
    break;
    case QUALIF2:
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             QUALIFICATIONS 2                 				 |\n");
    break;
    case QUALIF3:
    printf("__________________________________________________________________________________________________________\n");
    printf("|                                             QUALIFICATIONS 3                 				 |\n");
    break;	
    case FINALE:
    printf("______________________________________________________________________________________________________________________\n");
    printf("|                                               COURSE FINALE                     			             |\n");
    break;
  }
  int nbrCars;
  switch (pick)
  {
  case QUALIF1:
    nbrCars = 20;
    break;
  case QUALIF2:
    nbrCars = 15;
    break;
  case QUALIF3:
    nbrCars = 10;
    break;
  default:
    nbrCars = 20;
    break;
  }
        
  if(final == 1){ //------------AFFICHE LA FINALE-------------------------
	printf("|____________________________________________________________________________________________________________________|\n");
  	printf("| Car  | Best Tour   |  Tour Actuel     |   Section 1  |   Section 2  |   Section 3  |  Pit  |  Out  | Nb de lap|\n");
  	printf("|----------|-------------|------------------|--------------|--------------|--------------|-------|-------|-----------|\n");
    for(int i = 0; i < nbrCars; i++){
        printf("|   %2d     |  %.3f\"   |      %.3f\"    |    %.3f\"   |    %.3f\"   |   %.3f\"    |   %d   |  %d    |   %2.0f      |\n", mem.carTab[i].numero, mem.carTab[i].best[3], mem.carTab[i].currentLap, mem.carTab[i].sections[0], mem.carTab[i].sections[1], mem.carTab[i].sections[2], mem.carTab[i].pit, mem.carTab[i].out, mem.carTab[i].tourNbr);
        printf("|----------|-------------|------------------|--------------|--------------|--------------|-------|-------|-----------|\n");
    }
  }
  else{ //----------------------AFFICHE LES QUALS/ESSAIS--------------
	printf("|________________________________________________________________________________________________________|\n");
  	printf("| Car  | Best Tour   |  Tour Actuel     |   Section 1  |   Section 2  |   Section 3  |  Pit  |  Out  |\n");
  	printf("|----------|-------------|------------------|--------------|--------------|--------------|-------|-------|\n");
    for(int i = 0; i < nbrCars; i++){
        printf("|   %2d     |  %.3f\"   |    %3.3f\"      |     %2.3f\"  |     %2.3f\"  |    %2.3f\"   |   %d   |   %d   |\n", mem.carTab[i].numero,mem.carTab[i].best[3], mem.carTab[i].currentLap, mem.carTab[i].sections[0],mem.carTab[i].sections[1], mem.carTab[i].sections[2], mem.carTab[i].pit, mem.carTab[i].out);
        printf("|----------|-------------|------------------|--------------|--------------|--------------|-------|-------|\n");
    }
  }
  printf("__________________________________________________________________________________________________________\n");
  printf("|                                             Best                                   		         |\n");
  printf("|________________________________________________________________________________________________________|\n");

  printf("|  Best    | Section 1    |\n");
  printf("|----------|--------------|\n");
  carSort(memoryPoint->carTab, 0);
  printf("|   %2d     |  %.3f\"     |\n", mem.carTab[0].numero, mem.carTab[0].best[0]);
  printf("|----------|--------------|\n");
  printf("|  Best    | Section 2    |\n");
  printf("|----------|--------------|\n");
  carSort(memoryPoint->carTab, 1);
  printf("|   %2d     |  %.3f\"     |\n", mem.carTab[0].numero, mem.carTab[0].best[1]);
  printf("|----------|--------------|\n");
  printf("|  Best    | Section 3    |\n");
  printf("|----------|--------------|\n");
  carSort(memoryPoint->carTab, 2);
  printf("|   %2d     |  %.3f\"     |\n", mem.carTab[0].numero, mem.carTab[0].best[2]);
  printf("|----------|--------------|\n");
}

void qualifSort(struct SharedMemory *carsTab, int pick){
  carSort(carsTab->carTab, 3); //tri de sorte que les voitures qui ont finies restent à leur place dans le tableau
  int nbrCars;
  switch (pick)
  {
  case QUALIF1:
    nbrCars = 20;
    break;
  case QUALIF2:
    nbrCars = 15;
    break;
  case QUALIF3:
    nbrCars = 10;
    break;
  default:
    nbrCars = 20;
    break;
  }
  switch (nbrCars) {
    case 20:
    for (int i = 15; i < 20; i++) {
      carsTab->finalRace[i] = carsTab->carTab[i]; 
      carVarInit(&carsTab->carTab[i]); 
      carsTab->carTab[i].best[3] = i + 1000;
    }
    break;
    case 15: 
    for (int i = 10; i < 15; i++) {
      carsTab->finalRace[i] = carsTab->carTab[i];
      carVarInit(&carsTab->carTab[i]);
      carsTab->carTab[i].best[3] = i + 1000;
    }
    break;
    case 10:
    for (int i = 0; i < 10; i++) {
      carsTab->finalRace[i] = carsTab->carTab[i];
      carVarInit(&carsTab->carTab[i]);
      carsTab->carTab[i].best[3] = i + 1000;
    }
    break;
  }

}

void daddyFunction(struct SharedMemory *carsTab, int pick){
  int nbrFinishedCalc;
  switch (pick)
  {
  case QUALIF1:
    nbrFinishedCalc = 20;
    break;
  case QUALIF2:
    nbrFinishedCalc = 15;
    break;
  case QUALIF3:
    nbrFinishedCalc = 10;
    break;

  default:
    nbrFinishedCalc = 20;
    break;
  }
  while(carsTab->nbrCarFinished <nbrFinishedCalc){
    msleep(500);
    tabDisplay(carsTab, pick);
  }
  switch (pick)
  {
  case QUALIF1:
  case QUALIF2:
  case QUALIF3:
    qualifSort(carsTab, pick);
    break;
  
  default:
    break;
  }
  recapFile(carsTab, pick); //Pour avoir un document récapitulatif du tableau final
}

//Lance le programme
int main(int argc, char *argv[]){
  struct SharedMemory *carsTab;
  sem_init(&semaphore, 0, 1); // 0 indique que le sémaphore est local à ce processus, 1 est la valeur initiale du sémaphore

  int shmid = shmget(1009, sizeof(carsTab), IPC_CREAT | 0666);
  carsTab = shmat(shmid, NULL, 0);
  int pick = 0;

  printf("Quelle est la taille du circuit ? (en km (int))");
  int circuitSize;
  scanf("%d", &circuitSize);

  carsTab->lapNbrToDo = nbrTourCalc(circuitSize);
	//printf("test: %d", carsTab->lapNbrToDo); //Nombre de lap à faire

  while(pick != 10){
    carsTab->nbrCarFinished = 0;

  	printf("Que voulez-vous faire? \n");
	printf("Pour lancer les essais libres 1  (P1)  :0\n");
	printf("Pour lancer les essais libres 2  (P2)  :1\n");
	printf("Pour lancer les essais libres 3  (P3)  :2\n");
	printf("Pour lancer les qualifications 1 (Q1)  :3\n");
	printf("Pour lancer les qualifications 2 (Q2)  :4\n");
	printf("Pour lancer les qualifications 3 (Q3)  :5\n");
	printf("Pour lancer la course finale           :6\n");
	printf("Pour quitter le programme              :10\n");
    scanf("%d", &pick);

    switch(pick){
      case ESSAI1 : // P1 -> 1h30
      for(int i = 0; i < 20; i++){
        int pid = fork();
        if (pid < 0) {
          perror("bah zut ça forke pas\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 5400);
        }
      }
      daddyFunction(carsTab, pick); // C'est le père qui affiche (loïc : j'ai mis 0 explique moi si c'est pas ça mdr)
      break;
      case ESSAI2 : // P2 -> 1h30
      for(int i = 0; i < 20; i++){
        int pid = fork();
        if (pid < 0) {
          perror("ça forke pasencore aha\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 5400);
        }
      }
      daddyFunction(carsTab, pick); 
      break;
      case ESSAI3 : //P3 -> 1h
      for(int i = 0; i < 20; i++){
        int pid = fork();
        if (pid < 0) {
          perror("ça forke pas nul\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 3600);
        }
      }
      daddyFunction(carsTab, pick);
      break;
      case QUALIF1 :
      for(int i = 0; i < 20; i++){
        int pid = fork();
        if (pid < 0) {
          perror("ça forke paaaaaas\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 1080);
        }
      }
      daddyFunction(carsTab, pick);
      break;
      case QUALIF2 :
      for(int i = 0; i < 15; i++){
        int pid = fork();
        if (pid < 0) {
          perror("encore, ça forke pas\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 900);
        }
      }
      daddyFunction(carsTab, pick);
      break;
      case QUALIF3 :
      for(int i = 0; i < 10; i++){
        int pid = fork();
        if (pid < 0) {
          perror("ça forke toujours pas\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          fpAndQualif(carsTab, i, 720);
        }
      }
      daddyFunction(carsTab, pick);
      break;
      case FINALE :
      for(int i = 0; i < 20; i++){
        int pid = fork();
        if (pid < 0) {
          perror("ça forke pas\n");
          return -1;
        }
        else if (pid == 0) {
          srand(getpid());
          finalRace(carsTab, i);
        }
      }
      daddyFunction(carsTab, pick);
      break;
    }
  }
  sem_destroy(&semaphore);
  return 0;
}


