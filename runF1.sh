#!/bin/bash

# Demander combien de fois la boucle doit être exécutée
echo "Combien de GP voulez-vous lancer ?"
read iterations

for (( count=1; count<=$iterations; count++ ))
do
    # Lancer le programme F1 et interagir avec lui
    {
        # Attendre un peu pour que le programme se lance correctement
        sleep 2

        # Entrer une valeur aléatoire pour la taille du circuit entre 10 et 23
        echo $((RANDOM % 8 + 3))

        # Attendre que le menu s'affiche
        sleep 2

        # Sélectionner chaque option du menu
        for i in {0..6}
        do
            echo $i
            sleep 35
        done

        # Quitter le programme
        echo 10

    } | ./F1

    # Déplacer tous les fichiers `.txt` générés vers un dossier nommé en fonction de l'itération
    mkdir "GP$count"
    mv *.txt "GP$count/"
done
