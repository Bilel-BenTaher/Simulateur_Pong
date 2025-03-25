# README - Jeu de Pong en C++

## Description
Ce programme est une implémentation du jeu classique Pong en C++ utilisant la console Windows. Il propose une version améliorée avec des fonctionnalités supplémentaires comme un menu de paramètres, des effets sonores et une interface colorée.

## Fonctionnalités
- **Deux joueurs** : Contrôlez les raquettes avec les touches Z/S (Joueur 1) et O/L (Joueur 2)
- **Système de score** : Premier joueur à atteindre le score maximum gagne
- **Paramètres configurables** :
  - Modification du score maximum
  - Affichage des contrôles
- **Effets sonores** : Sons différents pour les rebonds et les points marqués
- **Interface graphique** :
  - Bordures colorées
  - Raquettes et balle visibles
  - Affichage des scores en temps réel
- **Menu pause** : Mettre le jeu en pause avec la touche P
- **Redimensionnement automatique** de la console pour une meilleure expérience

## Aperçu du jeu
![Aperçu du jeu Pong](images/pong_game.png)

## Contrôles
- **Joueur 1** : 
  - Monter : Z
  - Descendre : S
- **Joueur 2** : 
  - Monter : O
  - Descendre : L
- **Pause** : P
- **Paramètres** : W
- **Quitter** : Q

## Configuration requise
- Système d'exploitation Windows
- Compilateur C++ compatible avec les bibliothèques Windows.h et conio.h

## Compilation et exécution
1. Compilez le code avec un compilateur C++ (comme g++ ou Visual Studio)
2. Exécutez le fichier compilé
3. La console se redimensionnera automatiquement pour le jeu

## Paramètres
Appuyez sur W pendant le jeu pour accéder au menu des paramètres où vous pouvez :
- Augmenter/diminuer le score maximum avec +/-
- Voir les contrôles actuels
- Quitter les paramètres avec Entrée

## Auteur
Ce programme a été développé en C++ en utilisant les API Windows pour la gestion de la console.

## Licence
Ce code est fourni à titre éducatif et peut être librement utilisé et modifié.
