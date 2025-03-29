#include "GameManager.h"
#include <iostream>
#include <time.h>
#include <string>
#include <windows.h>  // Pour les fonctions de console Windows

/**
 * @brief Constructeur du gestionnaire de jeu
 * @param w Largeur du terrain de jeu
 * @param h Hauteur du terrain de jeu
 * 
 * Initialise tous les composants du jeu (ball, paddles) et configure
 * la console pour l'affichage.
 */
cGameManager::cGameManager(int w, int h) {
    srand(time(NULL));  // Initialisation du générateur aléatoire
    quit = false;       // Initialisation des flags
    paused = false;
    inSettings = false;
    
    // Configuration des contrôles par défaut
    up1 = 'z'; up2 = 'o';  // Touches pour monter
    down1 = 's'; down2 = 'l';  // Touches pour descendre
    
    score1 = score2 = 0;  // Initialisation des scores
    maxScore = 3;         // Score maximum par défaut
    width = w;            // Dimensions du terrain
    height = h;
    
    // Création des objets du jeu
    ball = new cBall(w / 2, h / 2);  // Balle au centre
    player1 = new cPaddle(1, h / 2 - 3, h);  // Joueur 1 à gauche
    player2 = new cPaddle(w - 2, h / 2 - 3, h);  // Joueur 2 à droite
    
    // Configuration du buffer de console
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    bufferSize = { (SHORT)width, (SHORT)height };
    characterPos = { 0, 0 };
    writeArea = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    consoleBuffer = new CHAR_INFO[width * height];
    
    // Masquage du curseur console
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

/**
 * @brief Destructeur du gestionnaire de jeu
 * 
 * Libère la mémoire allouée pour les objets du jeu
 * et le buffer de console.
 */
cGameManager::~cGameManager() {
    delete ball;
    delete player1;
    delete player2;
    delete[] consoleBuffer;
}

/**
 * @brief Change la couleur d'affichage dans la console
 * @param color Code couleur Windows (voir Constants.h)
 */
void cGameManager::setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

/**
 * @brief Dessine un caractère dans le buffer
 * @param x Position horizontale (0 à width-1)
 * @param y Position verticale (0 à height-1)
 * @param c Caractère à dessiner
 * @param color Couleur du caractère
 */
void cGameManager::drawChar(int x, int y, char c, int color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        consoleBuffer[y * width + x].Char.AsciiChar = c;
        consoleBuffer[y * width + x].Attributes = color;
    }
}

/**
 * @brief Nettoie le buffer de console
 * 
 * Remplit le buffer avec des espaces et la couleur par défaut.
 */
void cGameManager::clearBuffer() {
    for (int i = 0; i < width * height; i++) {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 0;
    }
}

/**
 * @brief Affiche le buffer dans la console
 * 
 * Transfère le contenu du buffer mémoire vers la console visible.
 */
void cGameManager::renderBuffer() {
    WriteConsoleOutputA(hConsole, consoleBuffer, bufferSize, characterPos, &writeArea);
}

/**
 * @brief Dessine un bouton dans l'interface
 * @param x Position horizontale du bouton
 * @param y Position verticale du bouton
 * @param text Texte du bouton
 * @param selected Si true, le bouton est mis en surbrillance
 */
void cGameManager::drawButton(int x, int y, const std::string& text, bool selected) {
    int color = selected ? COL_BUTTON | FOREGROUND_INTENSITY : COL_BUTTON;
    for (size_t i = 0; i < text.length(); i++) {
        drawChar(x + i, y, text[i], color);
    }
}

/**
 * @brief Affiche le menu des paramètres
 * 
 * Dessine une interface permettant de modifier :
 * - Le score maximum
 * - Les contrôles des joueurs
 */
void cGameManager::showSettings() {
    // Dimensions du menu
    int settingsWidth = 30;
    int settingsHeight = 15;
    int startX = (width - settingsWidth) / 2;
    int startY = (height - settingsHeight) / 2;
    
    // Fond du menu
    for (int x = startX + 1; x < startX + settingsWidth - 1; x++) {
        for (int y = startY + 1; y < startY + settingsHeight - 1; y++) {
            drawChar(x, y, ' ', COL_SETTINGS);
        }
    }
    
    // Bordures du menu
    for (int x = startX; x < startX + settingsWidth; x++) {
        drawChar(x, startY, 205, COL_SETTINGS | FOREGROUND_INTENSITY);
        drawChar(x, startY + settingsHeight - 1, 205, COL_SETTINGS | FOREGROUND_INTENSITY);
    }
    
    for (int y = startY + 1; y < startY + settingsHeight - 1; y++) {
        drawChar(startX, y, 186, COL_SETTINGS | FOREGROUND_INTENSITY);
        drawChar(startX + settingsWidth - 1, y, 186, COL_SETTINGS | FOREGROUND_INTENSITY);
    }
    
    // Coins du menu
    drawChar(startX, startY, 201, COL_SETTINGS | FOREGROUND_INTENSITY);
    drawChar(startX + settingsWidth - 1, startY, 187, COL_SETTINGS | FOREGROUND_INTENSITY);
    drawChar(startX, startY + settingsHeight - 1, 200, COL_SETTINGS | FOREGROUND_INTENSITY);
    drawChar(startX + settingsWidth - 1, startY + settingsHeight - 1, 188, COL_SETTINGS | FOREGROUND_INTENSITY);
    
    // Titre du menu
    std::string title = "PARAMETRES";
    for (size_t i = 0; i < title.length(); i++) {
        drawChar(startX + (settingsWidth - title.length()) / 2 + i, startY + 1, title[i], COL_TEXT | FOREGROUND_INTENSITY);
    }
    
    // Affichage du score maximum
    std::string scoreText = "Score max: " + std::to_string(maxScore);
    for (size_t i = 0; i < scoreText.length(); i++) {
        drawChar(startX + 2 + i, startY + 3, scoreText[i], COL_TEXT);
    }
    
    // Boutons pour modifier le score maximum
    drawButton(startX + 15, startY + 3, "+");
    drawButton(startX + 18, startY + 3, "-");
    
    // Affichage des contrôles
    std::string controlsTitle = "Commandes:";
    for (size_t i = 0; i < controlsTitle.length(); i++) {
        drawChar(startX + 2 + i, startY + 5, controlsTitle[i], COL_TEXT | FOREGROUND_INTENSITY);
    }
    
    std::string p1Controls = "J1: " + std::string(1, up1) + "/" + std::string(1, down1);
    std::string p2Controls = "J2: " + std::string(1, up2) + "/" + std::string(1, down2);
    
    for (size_t i = 0; i < p1Controls.length(); i++) {
        drawChar(startX + 2 + i, startY + 7, p1Controls[i], COL_TEXT);
    }
    
    for (size_t i = 0; i < p2Controls.length(); i++) {
        drawChar(startX + 2 + i, startY + 8, p2Controls[i], COL_TEXT);
    }
    
    // Bouton OK
    drawButton(startX + (settingsWidth - 4) / 2, startY + settingsHeight - 3, "OK", true);
}

/**
 * @brief Incrémente le score d'un joueur
 * @param player Pointeur vers le joueur qui marque
 * 
 * Vérifie si le score maximum est atteint et réinitialise
 * les positions si nécessaire.
 */
void cGameManager::ScoreUp(cPaddle* player) {
    if (player == player1)
        score1++;
    else if (player == player2)
        score2++;

    // Vérifie si un joueur a gagné
    if (score1 >= maxScore || score2 >= maxScore) {
        quit = true;
        return;
    }

    // Réinitialisation des positions
    ball->Reset();
    player1->Reset();
    player2->Reset();
}

/**
 * @brief Dessine l'ensemble du jeu dans le buffer
 * 
 * Gère l'affichage de :
 * - La balle et des raquettes
 * - Des bordures et de la ligne centrale
 * - Des scores et des informations de jeu
 * - Des menus (pause et paramètres)
 */
void cGameManager::Draw() {
    clearBuffer();  // Nettoie le buffer
    
    // Bordures du terrain
    for (int i = 0; i < width; i++) {
        drawChar(i, 0, 205, COL_BORDER);         // Bordure haute
        drawChar(i, height - 1, 205, COL_BORDER); // Bordure basse
    }
    
    for (int i = 0; i < height; i++) {
        drawChar(0, i, 186, COL_BORDER);         // Bordure gauche
        drawChar(width - 1, i, 186, COL_BORDER); // Bordure droite
    }
    
    // Coins du terrain
    drawChar(0, 0, 201, COL_BORDER);             // Coin haut-gauche
    drawChar(width - 1, 0, 187, COL_BORDER);     // Coin haut-droite
    drawChar(0, height - 1, 200, COL_BORDER);    // Coin bas-gauche
    drawChar(width - 1, height - 1, 188, COL_BORDER); // Coin bas-droite
    
    // Ligne centrale pointillée
    for (int i = 1; i < height - 1; i++) {
        if (i % 2 == 1)
            drawChar(width / 2, i, 186, COL_BORDER);
    }
    
    // Affichage de la balle
    drawChar(ball->getX(), ball->getY(), 'O', COL_BALL);
    
    // Affichage de la raquette du joueur 1 (4 segments)
    for (int i = 0; i < 4; i++) {
        int py = player1->getY() + i;
        drawChar(player1->getX(), py, 219, COL_PADDLE);
    }
    
    // Affichage de la raquette du joueur 2 (4 segments)
    for (int i = 0; i < 4; i++) {
        int py = player2->getY() + i;
        drawChar(player2->getX(), py, 219, COL_PADDLE);
    }
    
    // Affichage des scores
    std::string scoreText1 = "P1: " + std::to_string(score1);
    for (size_t i = 0; i < scoreText1.length(); i++) {
        drawChar(2 + i, 0, scoreText1[i], COL_SCORE);
    }
    
    std::string scoreText2 = "P2: " + std::to_string(score2);
    for (size_t i = 0; i < scoreText2.length(); i++) {
        drawChar(width - scoreText2.length() - 2 + i, 0, scoreText2[i], COL_SCORE);
    }
    
    std::string maxScoreText = "Max: " + std::to_string(maxScore);
    int maxScorePos = (width - maxScoreText.length()) / 2;
    for (size_t i = 0; i < maxScoreText.length(); i++) {
        drawChar(maxScorePos + i, 0, maxScoreText[i], COL_SCORE);
    }
    
    // Boutons d'interface
    drawButton(width - 10, height - 1, "Pause (P)");
    drawButton(2, height - 1, "Param (W)");
    
    // Message de pause
    if (paused && !inSettings) {
        std::string pauseMsg = "PAUSED - Press P to resume";
        int pausePos = (width - pauseMsg.length()) / 2;
        for (size_t i = 0; i < pauseMsg.length(); i++) {
            drawChar(pausePos + i, height / 2, pauseMsg[i], COL_PAUSE);
        }
    }
    
    // Affichage du menu des paramètres si actif
    if (inSettings) {
        showSettings();
    }
    
    renderBuffer();  // Affiche le buffer dans la console
}

/**
 * @brief Gère les entrées utilisateur
 * 
 * Traite les commandes pour :
 * - Déplacer les raquettes
 * - Mettre en pause le jeu
 * - Afficher les paramètres
 * - Quitter le jeu
 */
void cGameManager::Input() {
    if (_kbhit()) {
        char current = _getch();
        
        // Gestion des entrées dans le menu des paramètres
        if (inSettings) {
            if (current == 13) { // Touche Entrée
                inSettings = false;
            } else if (current == '+') {
                maxScore++;
            } else if (current == '-') {
                if (maxScore > 1) maxScore--;
            }
            return;
        }
        
        // Contrôles des joueurs
        if (current == up1)
            player1->moveUp();
        if (current == up2)
            player2->moveUp();
        if (current == down1)
            player1->moveDown();
        if (current == down2)
            player2->moveDown();
            
        // Démarrer le jeu si la balle est immobile
        if (ball->getDirection() == STOP)
            ball->randomDirection();
            
        // Commandes spéciales
        if (current == 'q')  // Quitter
            quit = true;
        if (current == 'p')  // Pause
            paused = !paused;
        if (current == 'w')  // Paramètres
            inSettings = true;
    }
}

/**
 * @brief Gère la logique du jeu
 * 
 * Effectue les calculs pour :
 * - Déplacement de la balle
 * - Collisions avec les raquettes et les bords
 * - Gestion des scores
 */
void cGameManager::Logic() {
    if (paused || inSettings) return;  // Ne rien faire si pause ou paramètres
    
    ball->Move();  // Déplace la balle
    
    // Récupération des positions
    int ballx = ball->getX();
    int bally = ball->getY();
    int player1x = player1->getX();
    int player2x = player2->getX();
    int player1y = player1->getY();
    int player2y = player2->getY();

    // Collision avec la raquette gauche (joueur 1)
    for (int i = 0; i < 4; i++) {
        if (ballx == player1x + 1 && bally == player1y + i) {
            // Rebond vers la droite (direction aléatoire)
            ball->changeDirection((eDir)((rand() % 3) + 4));
            Beep(500, 50);  // Son de rebond
        }
    }
    
    // Collision avec la raquette droite (joueur 2)
    for (int i = 0; i < 4; i++) {
        if (ballx == player2x - 1 && bally == player2y + i) {
            // Rebond vers la gauche (direction aléatoire)
            ball->changeDirection((eDir)((rand() % 3) + 1));
            Beep(500, 50);  // Son de rebond
        }
    }
    
    // Collision avec le bord inférieur
    if (bally == height - 2) {
        ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
        Beep(300, 50);  // Son de rebond
    }
    
    // Collision avec le bord supérieur
    if (bally == 1) {
        ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        Beep(300, 50);  // Son de rebond
    }
    
    // La balle sort à gauche (joueur 2 marque)
    if (ballx == 1) {
        ScoreUp(player2);
        Beep(1000, 200);  // Son de but
    }
    
    // La balle sort à droite (joueur 1 marque)
    if (ballx == width - 2) {
        ScoreUp(player1);
        Beep(1000, 200);  // Son de but
    }
}

/**
 * @brief Boucle principale du jeu
 * 
 * Exécute en continu les étapes du jeu :
 * 1. Affichage (Draw)
 * 2. Entrées (Input)
 * 3. Logique (Logic)
 * jusqu'à ce que le jeu soit quitté.
 */
void cGameManager::Run() {
    while (!quit) {
        Draw();
        Input();
        if (!paused && !inSettings) {
            Logic();
        }
        Sleep(30); // Contrôle de la vitesse du jeu (30ms)
    }
}

/**
 * @brief Détermine le gagnant de la partie
 * @return 1 si le joueur 1 a gagné, 2 si le joueur 2 a gagné, 0 sinon
 */
int cGameManager::getWinner() const {
    if (score1 >= maxScore) return 1;
    if (score2 >= maxScore) return 2;
    return 0;
}