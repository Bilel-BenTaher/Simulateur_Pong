#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Ball.h"
#include "Paddle.h"
#include "Constants.h"
#include <conio.h>      // Pour _kbhit() et _getch()
#include <windows.h>    // Pour les fonctions de console Windows
#include <sstream>      // Pour la manipulation de chaînes

/**
 * @class cGameManager
 * @brief Classe principale gérant la logique et l'affichage du jeu Pong
 * 
 * Contrôle le déroulement du jeu, les entrées utilisateur, les collisions
 * et l'affichage en utilisant un buffer de console pour un rendu fluide.
 */
class cGameManager {
private:
    // Dimensions du terrain
    int width, height;   
    
    // Scores des joueurs
    int score1, score2;  
    int maxScore;        // Score maximum pour gagner
    
    // Contrôles des joueurs
    char up1, down1;     // Touches joueur 1
    char up2, down2;     // Touches joueur 2
    
    // États du jeu
    bool quit;           // Flag pour quitter le jeu
    bool paused;         // Flag pour mettre en pause
    bool inSettings;     // Flag pour le menu des paramètres
    
    // Objets du jeu
    cBall* ball;         // Pointeur vers la balle
    cPaddle* player1;    // Pointeur vers le joueur 1
    cPaddle* player2;    // Pointeur vers le joueur 2
    
    // Gestion de la console Windows
    HANDLE hConsole;     // Handle de la console
    COORD bufferSize;    // Taille du buffer
    COORD characterPos;  // Position du curseur
    SMALL_RECT writeArea;// Zone d'écriture
    CHAR_INFO* consoleBuffer; // Buffer pour le rendu
    
    /**
     * @brief Change la couleur d'affichage dans la console
     * @param color Code couleur Windows (voir Constants.h)
     */
    void setColor(int color);
    
    /**
     * @brief Dessine un caractère dans le buffer
     * @param x Position horizontale
     * @param y Position verticale
     * @param c Caractère à dessiner
     * @param color Couleur du caractère
     */
    void drawChar(int x, int y, char c, int color);
    
    /**
     * @brief Nettoie le buffer de console
     */
    void clearBuffer();
    
    /**
     * @brief Affiche le buffer dans la console
     */
    void renderBuffer();
    
    /**
     * @brief Dessine un bouton dans l'interface
     * @param x Position horizontale
     * @param y Position verticale
     * @param text Texte du bouton
     * @param selected Si true, met le bouton en surbrillance
     */
    void drawButton(int x, int y, const std::string& text, bool selected = false);
    
    /**
     * @brief Affiche le menu des paramètres
     */
    void showSettings();
    
public:
    /**
     * @brief Constructeur
     * @param w Largeur du terrain
     * @param h Hauteur du terrain
     */
    cGameManager(int w, int h);
    
    /**
     * @brief Destructeur
     */
    ~cGameManager();
    
    /**
     * @brief Incrémente le score d'un joueur
     * @param player Pointeur vers le joueur qui marque
     */
    void ScoreUp(cPaddle* player);
    
    /**
     * @brief Dessine l'ensemble du jeu dans le buffer
     */
    void Draw();
    
    /**
     * @brief Gère les entrées utilisateur
     */
    void Input();
    
    /**
     * @brief Gère la logique du jeu (collisions, etc.)
     */
    void Logic();
    
    /**
     * @brief Lance la boucle principale du jeu
     */
    void Run();
    
    /**
     * @brief Récupère le gagnant de la partie
     * @return 1 pour joueur 1, 2 pour joueur 2, 0 si pas de gagnant
     */
    int getWinner() const;
};

#endif // GAMEMANAGER_H