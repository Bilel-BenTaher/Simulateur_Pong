#include "Ball.h"
#include <iostream>
#include <stdlib.h>  // Pour rand()

/**
 * @brief Constructeur de la balle
 * @param posX Position horizontale initiale
 * @param posY Position verticale initiale
 * 
 * Initialise la balle avec une position de départ et une direction STOP.
 */
cBall::cBall(int posX, int posY) {
    originalX = posX;  // Sauvegarde la position initiale X
    originalY = posY;  // Sauvegarde la position initiale Y
    x = posX;          // Position actuelle X
    y = posY;          // Position actuelle Y
    direction = STOP;  // Direction initiale (immobile)
}

/**
 * @brief Réinitialise la balle à sa position d'origine
 * 
 * Replace la balle à ses coordonnées initiales et stoppe son mouvement.
 */
void cBall::Reset() {
    x = originalX;     // Réinitialise X
    y = originalY;     // Réinitialise Y
    direction = STOP;  // Arrête le mouvement
}

/**
 * @brief Change la direction de la balle
 * @param d Nouvelle direction (enum eDir)
 * 
 * Affecte une nouvelle direction à la balle selon le paramètre fourni.
 */
void cBall::changeDirection(eDir d) {
    direction = d;  // Met à jour la direction
}

/**
 * @brief Donne une direction aléatoire à la balle
 * 
 * Génère une direction aléatoire parmi les 6 directions possibles
 * (excluant STOP). Utilisé principalement au début du jeu.
 */
void cBall::randomDirection() {
    // Génère un nombre entre 1 et 6 correspondant aux directions de mouvement
    direction = (eDir)((rand() % 6) + 1);
}

/**
 * @brief Déplace la balle selon sa direction actuelle
 * 
 * Met à jour les coordonnées de la balle en fonction de sa direction.
 * Gère les 6 directions possibles + l'état STOP.
 */
void cBall::Move() {
    switch (direction) {
        case STOP: 
            break;  // Ne pas bouger
        case LEFT: 
            x--;    // Déplacement vers la gauche
            break;
        case RIGHT: 
            x++;    // Déplacement vers la droite
            break;
        case UPLEFT: 
            x--; y--;  // Diagonale haut-gauche
            break;
        case DOWNLEFT: 
            x--; y++;  // Diagonale bas-gauche
            break;
        case UPRIGHT: 
            x++; y--;  // Diagonale haut-droite
            break;
        case DOWNRIGHT: 
            x++; y++;  // Diagonale bas-droite
            break;
    }
}

/**
 * @brief Surcharge de l'opérateur << pour l'affichage
 * @param o Flux de sortie
 * @param c Balle à afficher
 * @return Flux de sortie modifié
 * 
 * Permet d'afficher les informations de la balle au format :
 * Ball [x,y][direction]
 */
std::ostream& operator<<(std::ostream& o, const cBall& c) {
    o << "Ball [" << c.getX() << "," << c.getY() << "][" << c.getDirection() << "]";
    return o;
}