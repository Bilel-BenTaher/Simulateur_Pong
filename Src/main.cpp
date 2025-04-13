#include "GameManager.h"
#include "Constants.h"
#include <iostream>
#include <windows.h>  // Pour les fonctions de console Windows

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
        std::cout << "  ";
    }
    
    return 0;  // Fin du programme
}