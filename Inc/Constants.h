#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <windows.h>  // Pour les définitions Windows nécessaires

/**
 * @file constants.h
 * @brief Définition des constantes et paramètres du jeu
 * 
 * Contient les énumérations, constantes et fonctions utilitaires
 * utilisées dans tout le projet.
 */

/**
 * @enum eDir
 * @brief Énumération des directions possibles pour la balle
 * 
 * Représente les 6 directions de mouvement possibles ainsi que l'état STOP
 */
enum eDir { 
    STOP = 0,       ///< Aucun mouvement
    LEFT = 1,       ///< Vers la gauche
    UPLEFT = 2,     ///< En diagonale haut-gauche
    DOWNLEFT = 3,   ///< En diagonale bas-gauche
    RIGHT = 4,      ///< Vers la droite
    UPRIGHT = 5,    ///< En diagonale haut-droite
    DOWNRIGHT = 6   ///< En diagonale bas-droite
};

// =============================================
// COULEURS DE LA CONSOLE
// =============================================

/**
 * @def COL_BORDER
 * @brief Couleur cyan pour les bordures du terrain
 */
const int COL_BORDER = 11;     // Cyan

/**
 * @def COL_BALL
 * @brief Couleur jaune pour la balle
 */
const int COL_BALL = 14;       // Jaune

/**
 * @def COL_PADDLE
 * @brief Couleur vert clair pour les raquettes
 */
const int COL_PADDLE = 10;     // Vert clair

/**
 * @def COL_SCORE
 * @brief Couleur rouge pour l'affichage du score
 */
const int COL_SCORE = 12;      // Rouge

/**
 * @def COL_TEXT
 * @brief Couleur blanc pour le texte standard
 */
const int COL_TEXT = 15;       // Blanc

/**
 * @def COL_PAUSE
 * @brief Couleur violet pour le menu pause
 */
const int COL_PAUSE = 13;      // Violet

/**
 * @def COL_BUTTON
 * @brief Couleur bleu clair pour les boutons
 */
const int COL_BUTTON = 9;      // Bleu clair

/**
 * @def COL_SETTINGS
 * @brief Couleur violet foncé pour les menus de configuration
 */
const int COL_SETTINGS = 5;    // Violet foncé

// =============================================
// FONCTIONS UTILITAIRES
// =============================================

/**
 * @brief Configure la taille de la fenêtre console
 * @param width Largeur souhaitée en colonnes
 * @param height Hauteur souhaitée en lignes
 * 
 * Adapte la taille de la console pour l'affichage optimal du jeu.
 * Fonction spécifique à Windows.
 */
void SetConsoleSize(int width, int height);

#endif // CONSTANTS_H