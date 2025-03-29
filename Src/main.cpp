#include "GameManager.h"
#include "Constants.h"
#include <iostream>
#include <windows.h>  // Pour les fonctions de console Windows

/**
 * @brief Configure la taille de la console Windows
 * @param width Largeur souhaitée en colonnes
 * @param height Hauteur souhaitée en lignes
 * 
 * Cette fonction :
 * 1. Ajuste la taille du buffer console
 * 2. Définit la taille de la fenêtre
 * 3. Masque le curseur
 * 4. Définit un titre pour la fenêtre
 */
void SetConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 1. Configuration de la taille du buffer
    COORD coord = { (SHORT)width, (SHORT)height };
    SetConsoleScreenBufferSize(hConsole, coord);
    
    // 2. Ajustement de la taille de la fenêtre
    SMALL_RECT rect = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    
    // 3. Masquage du curseur
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // 4. Définition du titre de la fenêtre
    SetConsoleTitleW(L"Pong Game - Parametrables");
}

/**
 * @brief Point d'entrée principal du programme
 * @return Code de sortie (0 pour succès)
 * 
 * Fonction principale qui :
 * 1. Configure la console
 * 2. Lance la boucle principale du jeu
 * 3. Gère les parties multiples
 * 4. Affiche les résultats et propose de rejouer
 */
int main() {
    // Configuration initiale de la console
    SetConsoleSize(60, 30);  // 60 colonnes, 30 lignes
    
    // Boucle principale du jeu (permet de rejouer)
    while (true) {
        // Création et lancement d'une nouvelle partie
        cGameManager c(40, 20);  // Terrain de 40x20
        c.Run();  // Lancement de la boucle de jeu
        
        // Positionnement du curseur pour l'affichage des résultats
        COORD pos = {10, 22};  // Position X,Y
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsole, pos);
        SetConsoleTextAttribute(hConsole, COL_TEXT);  // Couleur texte blanc
        
        // Affichage du message de fin de partie
        int winner = c.getWinner();
        if (winner > 0) {
            std::cout << "Player " << winner << " wins! Play again? (y/n): ";
        } else {
            std::cout << "Game Over! Play again? (y/n): ";
        }
        
        // Récupération du choix de l'utilisateur
        char choice;
        std::cin >> choice;
        if (tolower(choice) != 'y') break;  // Quitter si réponse différente de 'y'
        
        // Effacement du message précédent
        pos.Y = 22;
        SetConsoleCursorPosition(hConsole, pos);
        std::cout << "                                                  ";
    }
    
    return 0;  // Fin du programme
}