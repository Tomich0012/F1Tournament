#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define NB_VOITURES 20
#define NB_SECTEURS 3

typedef struct {
    int numero;
    int temps_secteurs[NB_SECTEURS];
    int meilleur_temps;
    bool aux_stands;
    bool out;
} Voiture;

// Prototypes
int generateTemps();
void afficheClassement(Voiture voitures[]);

int main() {
    Voiture voitures[NB_VOITURES] = {
        {44}, {63}, {1}, {11}, {55}, {16}, {4}, {3}, {14}, {31},
        {10}, {22}, {5}, {18}, {6}, {23}, {77}, {24}, {47}, {9}
    };
    
    // Init
    for (int i = 0; i < NB_VOITURES; i++) {
        voitures[i].meilleur_temps = INT_MAX;
        voitures[i].aux_stands = false;
        voitures[i].out = false;
        for (int j = 0; j < NB_SECTEURS; j++) {
            voitures[i].temps_secteurs[j] = 0;
        }
    }
    
    // Simulation d'une séance
    for (int i = 0; i < NB_VOITURES; i++) {
        int total_temps = 0;
        for (int j = 0; j < NB_SECTEURS; j++) {
            int temps = generateTemps();
            voitures[i].temps_secteurs[j] = temps;
            total_temps += temps;
        }
        if (total_temps < voitures[i].meilleur_temps) {
            voitures[i].meilleur_temps = total_temps;
        }
    }
    
    // Afficher le classement
    afficheClassement(voitures);
    
    return 0;
}

int generateTemps() {
    return rand() % 21 + 25; // Genere un nombre entre 25 et 45
}

void afficheClassement(Voiture voitures[]) {
    // Trier les voitures par meilleur temps
    for (int i = 0; i < NB_VOITURES - 1; i++) {
        for (int j = i + 1; j < NB_VOITURES; j++) {
            if (voitures[j].meilleur_temps < voitures[i].meilleur_temps) {
                Voiture tmp = voitures[i];
                voitures[i] = voitures[j];
                voitures[j] = tmp;
            }
        }
    }
    
    printf("Classement:\n");
    for (int i = 0; i < NB_VOITURES; i++) {
        int diff = i == 0 ? 0 : voitures[i].meilleur_temps - voitures[i - 1].meilleur_temps;
        printf("No %d : %d s (%s%s) (diff: %d s)\n", voitures[i].numero, voitures[i].meilleur_temps,
               voitures[i].aux_stands ? "P " : "",
               voitures[i].out ? "Out" : "",
               diff);
    }
}
