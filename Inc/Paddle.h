#ifndef PADDLE_H
#define PADDLE_H

#include <iostream>  // Pour std::ostream

/**
 * @class cPaddle
 * @brief Classe représentant une raquette dans le jeu Pong
 *
 * Gère la position et le mouvement des raquettes pour les joueurs,
 * ainsi que les collisions avec les bords du terrain.
 */
class cPaddle {
private:
    int x, y;            ///< Position actuelle de la raquette
    int originalX, originalY; ///< Position initiale (pour réinitialisation)
    int gameHeight;      ///< Hauteur du terrain de jeu (pour les limites)

public:
    /**
     * @brief Constructeur par défaut
     * Initialise la raquette en position (0,0)
     */
    cPaddle();

    /**
     * @brief Constructeur paramétré
     * @param posX Position horizontale initiale
     * @param posY Position verticale initiale
     * @param height Hauteur du terrain de jeu
     */
    cPaddle(int posX, int posY, int height);

    /**
     * @brief Réinitialise la raquette à sa position d'origine
     */
    inline void Reset() { x = originalX; y = originalY; }

    /**
     * @brief Getter pour la position X
     * @return Position horizontale actuelle
     */
    inline int getX() { return x; }

    /**
     * @brief Getter pour la position Y
     * @return Position verticale actuelle
     */
    inline int getY() { return y; }

    /**
     * @brief Déplace la raquette vers le haut
     * Vérifie les limites du terrain avant de déplacer
     */
    inline void moveUp() { if (y > 1) y--; }

    /**
     * @brief Déplace la raquette vers le bas
     * Vérifie les limites du terrain avant de déplacer
     * (3 correspond à la hauteur de la raquette)
     */
    inline void moveDown() { if (y + 3 < gameHeight - 2) y++; }

    /**
     * @brief Surcharge de l'opérateur << pour l'affichage
     * @param o Flux de sortie
     * @param c Raquette à afficher
     * @return Flux de sortie
     */
    friend std::ostream& operator<<(std::ostream& o, cPaddle c);
};

#endif // PADDLE_H