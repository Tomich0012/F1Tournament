#include "utils.h"

#define  ESSAI1  0
#define ESSAI2  1
#define ESSAI3  2
#define QUALIF1  3
#define QUALIF2  4
#define QUALIF3  5
#define FINALE  6
#define QUIT  10

int nbrTourCalc(int circuitSize){
  int res = NB_KM / circuitSize;
  return res;
}

int msleep(long tms){
  struct timespec ts;
  int ret;

  if (tms < 0)
  {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = tms / 1000;
  ts.tv_nsec = (tms % 1000) * 1000000;

  do {
    ret = nanosleep(&ts, &ts);
  } while (ret && errno == EINTR);

  return ret;
}

void swap(struct Car *x, struct Car *y){
  struct Car temp = *x;
  *x = *y;
  *y = temp;
}

void carSort(struct Car *cars, int pick){
  for (int i = 0; i < 19; i++){
    for (int j = 0; j < 19 - i; j++){
      if(pick == 5){
        if (cars[j].tourNbr < cars[j+1].tourNbr){
          swap(&cars[j], &cars[j+1]);
        }
      }
      else{
        if (cars[j].best[pick] > cars[j+1].best[pick]){
          swap(&cars[j], &cars[j+1]);
        }
      }
    }
  }
}

//Renvoie un entier random entre min et min + max
double randomInt(int max, int min){
  return (rand() % max) + min;
}

//Renvoie un temps double au centième
double randomTime(int min, int max){
  double entireP = randomInt(min, max);
  double decimalP = randomInt(999, 0);
  decimalP /= 1000;
  return entireP + decimalP;
}

double sectionCalc(){
  return randomTime(15, 35);
}

double bestTime(double t1, double t2){
  double theBest;
  if (t1 < t2) {
    theBest = t1;
  }
  else{
    theBest = t2;
  }
  return theBest;
}

double stand(){
  double addTime = 0.0;
  if (randomInt(100,0) >= 97) {
    addTime = randomTime(10, 10);
  }
  return addTime;
}

int out(){
  int result = 0;
  if (randomInt(100,0) >= 99) {
    result = 1;
  }
  return result;
}

void carVarInit(struct Car *v){
  for(int i = 0; i < 3; i++){
    v->best[i] = 51; //secondes par section
  }
  v->best[3] = 151;
  v->pit = 0;
  v->out = 0;
}

struct Car car_init(int i){
  int carNum[20] = {7,99,5,16,8,20,4,55,10,26,44,77,11,18,23,33,3,27,6,85};
  struct Car v;
  struct Car *carPoint = &v;
  v.numero = carNum[i];
  carVarInit(carPoint);
  return v;
}

void recapFile(struct SharedMemory *carsTab, int pick){
    FILE *file = NULL;
    switch (pick) {
        case ESSAI1:
            file = fopen("RecapEssai1.txt", "a");
            break;
	      case ESSAI2:
            file = fopen("RecapEssai2.txt", "a");
            break;
        case ESSAI3:
            file = fopen("RecapEssai3.txt", "a");
            break;
        case QUALIF1:
            file = fopen("RecapQualifications1.txt", "a");
            break;
        case QUALIF2:
            file = fopen("RecapQualifications2.txt", "a");
            break;
        case QUALIF3:
            file = fopen("RecapQualifications3.txt", "a");
            break;
        case FINALE:
            file = fopen("RecapCourseFinal.txt", "a");
            break;
    }
    if(file != NULL){
        struct SharedMemory mem = *carsTab;
        struct SharedMemory *memoryPoint = &mem;
        int final = 0;
        if(pick == FINALE){
          carSort(memoryPoint->carTab, 5);
          final = 1;
        }
        else{
          carSort(memoryPoint->carTab, 3);
        }
        system("clear");
        fprintf(file,"___________________________________\n");
        switch (pick) {
            case ESSAI1:
                fprintf(file,"|             FREE PRACTICE 1           |\n");
                break;
	          case ESSAI2:
                fprintf(file,"|             FREE PRACTICE 2           |\n");
                break;
            case ESSAI3:
                fprintf(file,"|             FREE PRACTICE 3           |\n");
                break;
            case QUALIF1:
                fprintf(file,"|                   Q 1                 |\n");
                break;
            case QUALIF2:
                fprintf(file,"|                   Q 2                |\n");
                break;
            case QUALIF3:
                fprintf(file,"|                   Q 3               |\n");
                break;
            case FINALE:
                fprintf(file,"|          FINAL RACE          |\n");
                break;
        }
        int nbrCars = 0;
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
        fprintf(file,"|_________________________________|\n");
        fprintf(file,"| Car |   Lap    | Pit | Out |\n");
        fprintf(file,"|---------|-----------|-----|-----|\n");
        if(final == 1){
          for(int i = 0; i < nbrCars; i++){
            fprintf(file,"|   %2d    |   %2.0f      |  %d  |  %d  |\n", mem.carTab[i].numero, (mem.carTab[i].tourNbr), mem.carTab[i].pit, mem.carTab[i].out);
            fprintf(file,"----------------------------------|\n");
          }
        }
        else{
          for(int i = 0; i < nbrCars; i++){
            fprintf(file,"|   %2d    |   %.3f\"  |  %d |  %d |\n", mem.carTab[i].numero, mem.carTab[i].best[3], mem.carTab[i].pit, mem.carTab[i].out);
            fprintf(file,"|---------------------------------|\n");
          }
        }
        fprintf(file,"|  Best   | Section 1 |\n");
        fprintf(file,"|---------|-----------|\n");
        carSort(memoryPoint->carTab, 0);
        fprintf(file,"|   %2d    |   %.3f\" |\n", mem.carTab[0].numero, mem.carTab[0].best[0]);
        fprintf(file,"|---------|-----------|\n");
        fprintf(file,"|  Best   | Section 2 |\n");
        fprintf(file,"|---------|-----------|\n");
        carSort(memoryPoint->carTab, 1);
        fprintf(file,"|   %2d    |   %.3f\" |\n", mem.carTab[0].numero, mem.carTab[0].best[1]);
        fprintf(file,"|---------|-----------|\n");
        fprintf(file,"|  Best   | Section 3 |\n");
        fprintf(file,"|---------|-----------|\n");
        carSort(memoryPoint->carTab, 2);
        fprintf(file,"|   %2d    |   %.3f\" |\n", mem.carTab[0].numero, mem.carTab[0].best[2]);
        fprintf(file,"|---------------------|\n");
        fclose(file);
    }
    else{
        printf("ooooops an error occured while opening the file\n");
    }
}