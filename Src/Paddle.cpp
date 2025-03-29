#include "Paddle.h"

/**
 * @brief Constructeur par défaut de la raquette
 * 
 * Initialise toutes les positions à 0 et la hauteur de jeu à 0.
 * Utilise la liste d'initialisation pour une meilleure performance.
 */
cPaddle::cPaddle() 
    : x(0), y(0), 
      originalX(0), originalY(0),
      gameHeight(0) 
{
    // Toutes les initialisations sont faites dans la liste d'initialisation
}

/**
 * @brief Constructeur paramétré de la raquette
 * @param posX Position horizontale initiale
 * @param posY Position verticale initiale
 * @param height Hauteur du terrain de jeu
 * 
 * Initialise la raquette avec :
 * - Une position de départ
 * - Une position de réinitialisation
 * - La hauteur du terrain pour les vérifications de limites
 */
cPaddle::cPaddle(int posX, int posY, int height) 
    : cPaddle()  // Délégation au constructeur par défaut
{
    originalX = posX;  // Position X de réinitialisation
    originalY = posY;  // Position Y de réinitialisation
    x = posX;          // Position X actuelle
    y = posY;          // Position Y actuelle
    gameHeight = height; // Hauteur du terrain pour les limites
}

/**
 * @brief Surcharge de l'opérateur << pour l'affichage
 * @param o Flux de sortie
 * @param c Raquette à afficher
 * @return Flux de sortie modifié
 * 
 * Permet d'afficher les informations de la raquette au format :
 * Paddle [x,y]
 * 
 * Note: Le paramètre pourrait être passé par référence constante (const cPaddle&)
 * pour optimiser les performances avec des objets plus complexes.
 */
std::ostream& operator<<(std::ostream& o, cPaddle c) {
    o << "Paddle [" << c.x << "," << c.y << "]";
    return o;
}