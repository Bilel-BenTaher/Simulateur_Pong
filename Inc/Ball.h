#ifndef BALL_H
#define BALL_H

#include <iostream>
#include "Constants.h"

/**
 * @class cBall
 * @brief Classe représentant la balle dans le jeu
 * 
 * Gère la position, le mouvement et les collisions de la balle
 * avec les bords du terrain et les raquettes.
 */
class cBall {
private:
    int x, y;               ///< Position actuelle de la balle
    int originalX, originalY; ///< Position initiale (pour réinitialisation)
    eDir direction;         ///< Direction actuelle du mouvement

public:
    /**
     * @brief Constructeur de la balle
     * @param posX Position horizontale initiale
     * @param posY Position verticale initiale
     */
    cBall(int posX, int posY);

    /**
     * @brief Réinitialise la balle à sa position d'origine
     */
    void Reset();

    /**
     * @brief Change la direction de la balle
     * @param d Nouvelle direction (enum eDir)
     */
    void changeDirection(eDir d);

    /**
     * @brief Donne une direction aléatoire à la balle
     */
    void randomDirection();

    /**
     * @brief Getter pour la position X
     * @return Position horizontale actuelle
     */
    inline int getX() const { return x; }

    /**
     * @brief Getter pour la position Y
     * @return Position verticale actuelle
     */
    inline int getY() const { return y; }

    /**
     * @brief Getter pour la direction
     * @return Direction actuelle (enum eDir)
     */
    inline eDir getDirection() const { return direction; }

    /**
     * @brief Déplace la balle selon sa direction actuelle
     */
    void Move();
    
    /**
     * @brief Surcharge de l'opérateur << pour l'affichage
     * @param o Flux de sortie
     * @param c Balle à afficher
     * @return Flux de sortie
     */
    friend std::ostream& operator<<(std::ostream& o, const cBall& c);
};

#endif // BALL_H