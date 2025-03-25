#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <sstream>

using namespace std;

enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };

// Définition des couleurs
const int COL_BORDER = 11;     // Cyan
const int COL_BALL = 14;       // Jaune
const int COL_PADDLE = 10;     // Vert clair
const int COL_SCORE = 12;      // Rouge
const int COL_TEXT = 15;       // Blanc
const int COL_PAUSE = 13;      // Violet
const int COL_BUTTON = 9;      // Bleu clair
const int COL_SETTINGS = 5;    // Violet foncé

class cBall {
private:
    int x, y;
    int originalX, originalY;
    eDir direction;
public:
    cBall(int posX, int posY) {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        direction = STOP;
    }
    
    void Reset() {
        x = originalX;
        y = originalY;
        direction = STOP;
    }
    
    void changeDirection(eDir d) {
        direction = d;
    }
    
    void randomDirection() {
        direction = (eDir)((rand() % 6) + 1);
    }
    
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline eDir getDirection() { return direction; }
    
    void Move() {
        switch (direction) {
            case STOP: break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UPLEFT: x--; y--; break;
            case DOWNLEFT: x--; y++; break;
            case UPRIGHT: x++; y--; break;
            case DOWNRIGHT: x++; y++; break;
        }
    }
    
    friend ostream& operator<<(ostream& o, cBall c) {
        o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
        return o;
    }
};

class cPaddle {
private:
    int x, y;
    int originalX, originalY;
    int gameHeight;
public:
    cPaddle() {
        x = y = 0;
        gameHeight = 0;
    }
    
    cPaddle(int posX, int posY, int height) : cPaddle() {
        originalX = posX;
        originalY = posY;
        x = posX;
        y = posY;
        gameHeight = height;
    }
    
    inline void Reset() { x = originalX; y = originalY; }
    inline int getX() { return x; }
    inline int getY() { return y; }
    inline void moveUp() { if (y > 1) y--; } // 1 = juste sous la bordure supérieure
    inline void moveDown() { if (y + 3 < gameHeight - 2) y++; } // gameHeight-2 = juste au-dessus de la bordure inférieure
    
    friend ostream& operator<<(ostream& o, cPaddle c) {
        o << "Paddle [" << c.x << "," << c.y << "]";
        return o;
    }
};

class cGameManager {
private:
    int width, height;
    int score1, score2;
    int maxScore;
    char up1, down1, up2, down2;
    bool quit;
    bool paused;
    bool inSettings;
    cBall* ball;
    cPaddle* player1;
    cPaddle* player2;
    
    HANDLE hConsole;
    COORD bufferSize, characterPos;
    SMALL_RECT writeArea;
    CHAR_INFO* consoleBuffer;
    
    void setColor(int color) {
        SetConsoleTextAttribute(hConsole, color);
    }
    
    void drawChar(int x, int y, char c, int color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            consoleBuffer[y * width + x].Char.AsciiChar = c;
            consoleBuffer[y * width + x].Attributes = color;
        }
    }
    
    void clearBuffer() {
        for (int i = 0; i < width * height; i++) {
            consoleBuffer[i].Char.AsciiChar = ' ';
            consoleBuffer[i].Attributes = 0;
        }
    }
    
    void renderBuffer() {
        WriteConsoleOutputA(hConsole, consoleBuffer, bufferSize, characterPos, &writeArea);
    }
    
    void drawButton(int x, int y, const string& text, bool selected = false) {
        int color = selected ? COL_BUTTON | FOREGROUND_INTENSITY : COL_BUTTON;
        for (size_t i = 0; i < text.length(); i++) {
            drawChar(x + i, y, text[i], color);
        }
    }
    
    void showSettings() {
        // Cadre des paramètres
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
        
        // Bordures
        for (int x = startX; x < startX + settingsWidth; x++) {
            drawChar(x, startY, 205, COL_SETTINGS | FOREGROUND_INTENSITY);
            drawChar(x, startY + settingsHeight - 1, 205, COL_SETTINGS | FOREGROUND_INTENSITY);
        }
        
        for (int y = startY + 1; y < startY + settingsHeight - 1; y++) {
            drawChar(startX, y, 186, COL_SETTINGS | FOREGROUND_INTENSITY);
            drawChar(startX + settingsWidth - 1, y, 186, COL_SETTINGS | FOREGROUND_INTENSITY);
        }
        
        // Coins
        drawChar(startX, startY, 201, COL_SETTINGS | FOREGROUND_INTENSITY);
        drawChar(startX + settingsWidth - 1, startY, 187, COL_SETTINGS | FOREGROUND_INTENSITY);
        drawChar(startX, startY + settingsHeight - 1, 200, COL_SETTINGS | FOREGROUND_INTENSITY);
        drawChar(startX + settingsWidth - 1, startY + settingsHeight - 1, 188, COL_SETTINGS | FOREGROUND_INTENSITY);
        
        // Titre
        string title = "PARAMETRES";
        for (size_t i = 0; i < title.length(); i++) {
            drawChar(startX + (settingsWidth - title.length()) / 2 + i, startY + 1, title[i], COL_TEXT | FOREGROUND_INTENSITY);
        }
        
        // Score maximum
        string scoreText = "Score max: " + to_string(maxScore);
        for (size_t i = 0; i < scoreText.length(); i++) {
            drawChar(startX + 2 + i, startY + 3, scoreText[i], COL_TEXT);
        }
        
        // Boutons +/-
        drawButton(startX + 15, startY + 3, "+");
        drawButton(startX + 18, startY + 3, "-");
        
        // Contrôles joueurs
        string controlsTitle = "Commandes:";
        for (size_t i = 0; i < controlsTitle.length(); i++) {
            drawChar(startX + 2 + i, startY + 5, controlsTitle[i], COL_TEXT | FOREGROUND_INTENSITY);
        }
        
        string p1Controls = "J1: " + string(1, up1) + "/" + string(1, down1);
        string p2Controls = "J2: " + string(1, up2) + "/" + string(1, down2);
        
        for (size_t i = 0; i < p1Controls.length(); i++) {
            drawChar(startX + 2 + i, startY + 7, p1Controls[i], COL_TEXT);
        }
        
        for (size_t i = 0; i < p2Controls.length(); i++) {
            drawChar(startX + 2 + i, startY + 8, p2Controls[i], COL_TEXT);
        }
        
        // Bouton OK
        drawButton(startX + (settingsWidth - 4) / 2, startY + settingsHeight - 3, "OK", true);
    }
    
public:
    cGameManager(int w, int h) {
        srand(time(NULL));
        quit = false;
        paused = false;
        inSettings = false;
        up1 = 'z'; up2 = 'o';
        down1 = 's'; down2 = 'l';
        score1 = score2 = 0;
        maxScore = 3;
        width = w; 
        height = h;
        
        ball = new cBall(w / 2, h / 2);
        player1 = new cPaddle(1, h / 2 - 3, h);
        player2 = new cPaddle(w - 2, h / 2 - 3, h);
        
        // Initialisation du buffer
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        bufferSize = { (SHORT)width, (SHORT)height };
        characterPos = { 0, 0 };
        writeArea = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
        consoleBuffer = new CHAR_INFO[width * height];
        
        // Masquer le curseur
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }
    
    ~cGameManager() {
        delete ball, player1, player2;
        delete[] consoleBuffer;
    }
    
    void ScoreUp(cPaddle* player) {
        if (player == player1)
            score1++;
        else if (player == player2)
            score2++;

        if (score1 >= maxScore || score2 >= maxScore) {
            quit = true;
            return;
        }

        ball->Reset();
        player1->Reset();
        player2->Reset();
    }
    
    void Draw() {
        clearBuffer();
        
        // Dessiner les bordures
        for (int i = 0; i < width; i++) {
            drawChar(i, 0, 205, COL_BORDER);
            drawChar(i, height - 1, 205, COL_BORDER);
        }
        
        for (int i = 0; i < height; i++) {
            drawChar(0, i, 186, COL_BORDER);
            drawChar(width - 1, i, 186, COL_BORDER);
        }
        
        // Coins
        drawChar(0, 0, 201, COL_BORDER);
        drawChar(width - 1, 0, 187, COL_BORDER);
        drawChar(0, height - 1, 200, COL_BORDER);
        drawChar(width - 1, height - 1, 188, COL_BORDER);
        
        // Ligne centrale
        for (int i = 1; i < height - 1; i++) {
            if (i % 2 == 1)
                drawChar(width / 2, i, 186, COL_BORDER);
        }
        
        // Ball
        drawChar(ball->getX(), ball->getY(), 'O', COL_BALL);
        
        // Joueur 1 (4 segments)
        for (int i = 0; i < 4; i++) {
            int py = player1->getY() + i;
            drawChar(player1->getX(), py, 219, COL_PADDLE);
        }
        
        // Joueur 2 (4 segments)
        for (int i = 0; i < 4; i++) {
            int py = player2->getY() + i;
            drawChar(player2->getX(), py, 219, COL_PADDLE);
        }
        
        // Score Player 1 (à gauche)
        string scoreText1 = "P1: " + to_string(score1);
        for (size_t i = 0; i < scoreText1.length(); i++) {
            drawChar(2 + i, 0, scoreText1[i], COL_SCORE);
        }
        
        // Score Player 2 (à droite)
        string scoreText2 = "P2: " + to_string(score2);
        for (size_t i = 0; i < scoreText2.length(); i++) {
            drawChar(width - scoreText2.length() - 2 + i, 0, scoreText2[i], COL_SCORE);
        }
        
        // Score maximum (au centre)
        string maxScoreText = "Max: " + to_string(maxScore);
        int maxScorePos = (width - maxScoreText.length()) / 2;
        for (size_t i = 0; i < maxScoreText.length(); i++) {
            drawChar(maxScorePos + i, 0, maxScoreText[i], COL_SCORE);
        }
        
        // Bouton Pause
        drawButton(width - 10, height - 1, "Pause (P)");
        
        // Bouton Paramètres
        drawButton(2, height - 1, "Param (W)");
        
        // Message de pause
        if (paused && !inSettings) {
            string pauseMsg = "PAUSED - Press P to resume";
            int pausePos = (width - pauseMsg.length()) / 2;
            for (size_t i = 0; i < pauseMsg.length(); i++) {
                drawChar(pausePos + i, height / 2, pauseMsg[i], COL_PAUSE);
            }
        }
        
        // Afficher les paramètres si nécessaire
        if (inSettings) {
            showSettings();
        }
        
        renderBuffer();
    }
    
    void Input() {
        if (_kbhit()) {
            char current = _getch();
            
            if (inSettings) {
                // Gestion des paramètres
                if (current == 13) { // Entrée
                    inSettings = false;
                } else if (current == '+') {
                    maxScore++;
                } else if (current == '-') {
                    if (maxScore > 1) maxScore--;
                }
                return;
            }
            
            // Contrôles des paddles avec limites
            if (current == up1)
                player1->moveUp();
            if (current == up2)
                player2->moveUp();
            if (current == down1)
                player1->moveDown();
            if (current == down2)
                player2->moveDown();
                
            if (ball->getDirection() == STOP)
                ball->randomDirection();
            if (current == 'q')
                quit = true;
            if (current == 'p')
                paused = !paused;
            if (current == 'w')
                inSettings = true;
        }
    }
    
    void Logic() {
        if (paused || inSettings) return;
        
        ball->Move();
        int ballx = ball->getX();
        int bally = ball->getY();
        int player1x = player1->getX();
        int player2x = player2->getX();
        int player1y = player1->getY();
        int player2y = player2->getY();

        // Rebond sur la raquette gauche
        for (int i = 0; i < 4; i++) {
            if (ballx == player1x + 1 && bally == player1y + i) {
                ball->changeDirection((eDir)((rand() % 3) + 4));
                Beep(500, 50);
            }
        }
        
        // Rebond sur la raquette droite
        for (int i = 0; i < 4; i++) {
            if (ballx == player2x - 1 && bally == player2y + i) {
                ball->changeDirection((eDir)((rand() % 3) + 1));
                Beep(500, 50);
            }
        }
        
        // Rebond sur le mur du bas
        if (bally == height - 2) {
            ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
            Beep(300, 50);
        }
        
        // Rebond sur le mur du haut
        if (bally == 1) {
            ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
            Beep(300, 50);
        }
        
        // Marqué à gauche (joueur 2 gagne un point)
        if (ballx == 1) {
            ScoreUp(player2);
            Beep(1000, 200);
        }
        
        // Marqué à droite (joueur 1 gagne un point)
        if (ballx == width - 2) {
            ScoreUp(player1);
            Beep(1000, 200);
        }
    }
    
    void Run() {
        while (!quit) {
            Draw();
            Input();
            if (!paused && !inSettings) {
                Logic();
            }
            Sleep(30); // Contrôle de la vitesse du jeu
        }
    }
    
    int getWinner() const {
        if (score1 >= maxScore) return 1;
        if (score2 >= maxScore) return 2;
        return 0;
    }
};

void SetConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Définir la taille du buffer
    COORD coord = { (SHORT)width, (SHORT)height };
    SetConsoleScreenBufferSize(hConsole, coord);
    
    // Définir la taille de la fenêtre
    SMALL_RECT rect = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    
    // Masquer le curseur
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // Définir le titre
    SetConsoleTitleW(L"Pong Game - Parametrables");
}

int main() {
    // Définir la taille fixe de la console (60x30 caractères)
    SetConsoleSize(60, 30);
    
    while (true) {
        // Taille du jeu (40x20 caractères, centré dans la console 60x30)
        cGameManager c(40, 20);
        c.Run();
        
        // Afficher le message de fin en dehors du buffer du jeu
        COORD pos = {10, 22};
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsole, pos);
        SetConsoleTextAttribute(hConsole, COL_TEXT);
        
        int winner = c.getWinner();
        if (winner > 0) {
            cout << "Player " << winner << " wins! Play again? (y/n): ";
        } else {
            cout << "Game Over! Play again? (y/n): ";
        }
        
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y') break;
        
        // Effacer la ligne du message
        pos.Y = 22;
        SetConsoleCursorPosition(hConsole, pos);
        cout << "                                                  ";
    }
    
    return 0;
}