//project-150-car simulation game :
#include<iostream>
#include<vector>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<fstream>
#include<algorithm>

// Define constants for screen dimensions
#define SCREEN_WIDTH 100  // Total width of the screen
#define SCREEN_HEIGHT 30 // Total height of the screen
#define WIN_WIDTH 70     // Width of the game window within the screen


using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];//represent the x and y position of the enemy
int enemyX[3];
int enemyFlag[3];//activation of the enemy and display
char car[4][4] = { ' ','+','+',' ',   // Top row of the car
                  '+','+','+','+',
                  ' ','+','+',' ',
                  '+','+','+','+'};    // Bottom row of the car

int carpos = WIN_WIDTH/2;
int score = 0;
string playerName;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 50;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;

    SetConsoleCursorInfo(console, &lpCursor);//api
}

void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 18; j++) {//draw the left and right border
            gotoxy(0 + j, i);//set the cursor to the current column and row
            cout << "|";
            gotoxy(WIN_WIDTH - j, i);
            cout << "|";
        }
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(SCREEN_WIDTH, i);
        cout << "|";
    }
}

void genEnemy(int ind) {
    enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carpos, i + 22); cout << car[i][j];
        }
    }
}

void eraseCar() {//for updating we should erase the car
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carpos, i + 22); cout << " ";
        }
    }
}

int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyX[0] + 4 - carpos >= 0 && enemyX[0] + 4 - carpos < 9) {//horizonatal position of the enemy align with the current carposition
            return 1;
        }
    }
    return 0;
}

void saveScore(int score) {//changing or updating the data
    ofstream scoreFile("scores.txt", ios::app);//declare output file stream namescoeFile and topen a file name score.txt in append mode

    if (scoreFile.is_open()) {
        scoreFile << playerName << " " << score << endl;
        scoreFile.close();
    }
}

void displayTopScores() {
    system("cls");
    cout << "\t\t----------------------------" << endl;
    cout << "\t\t------ Top 10 Scores --------" << endl;
    cout << "\t\t----------------------------" << endl;

    ifstream scoreFile("scores.txt");//declare inuput file stream name scorefile and read/open the score.txt file
    if (scoreFile.is_open()) {
        vector<pair<string, int>> players;//create vector pair for store player name and score
        string name;
        int s;
        while (scoreFile >> name >> s) {//read data from the file
            players.push_back({name, s});
        }

        players.push_back({playerName, score});//adding new element in the vector
        sort(players.begin(), players.end(), [](const auto &a, const auto &b) {
            return a.second > b.second;
        });

        int count = min(10, static_cast<int>(players.size()));//display how many variable will display
        for (int i = 0; i < count; ++i) {
            cout << "\t\t" << players[i].first << ": " << players[i].second << endl;
        }

        scoreFile.close();
    } else {
        cout << "Unable to open scores file." << endl;
    }

    cout << "\n\t\tPress any Key to go back to menu.";
    getch();
}

void displayPlayerScore() {
    system("cls");
    cout << "\t\t----------------------------" << endl;
    cout << "\t\t------ Player Score --------" << endl;
    cout << "\t\t----------------------------" << endl;
    cout << "\t\tPlayer Name: " << playerName << endl;
    cout << "\t\tYour Score: " << score << endl;
    cout << "\n\t\tPress any Key to go back to menu.";
    getch();
}

void readPlayerData(string playerName, int &score) {//read player data from score.txt file and it print to the console
    ifstream scoreFile("scores.txt");

    if (scoreFile.is_open()) {
        while (scoreFile >> playerName >> score) {//loop for read the payer data fromscorefile
            cout << "\t\t" << playerName << ": " << score << endl;
        }

        scoreFile.close();
    } else {
        cout << "Unable to open scores file." << endl;
    }
}

void gameover() {
    cout << endl;
    cout << "\t\t----------------------------" << endl;
    cout << "\t\t--------- Game Over --------" << endl;
    cout << "\t\t----------------------------" << endl;
    cout << "\t\tPlayer Name: " << playerName << endl;
    cout << "\t\tYour Score: " << score << endl;

    saveScore(score);
    displayPlayerScore();
    system("cls");
}

void updateScore() {//current score
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Score: " << score << endl;
}

void instructions() {
    system("cls");//console command to clear the console screen
    cout << "Instructions";
    cout << "\n-----------------";
    cout << "\n Avoid car by moving left or right.";
    cout << "\n\n Press 'Arrow Keys' to move the Car";
    cout << "\n\n Press 'escape' to Exit";
    cout << "\n\n Press any Key to go back to the menu";
    getch();//use to keypress
}

void play() {
    cout << "Enter your name: ";
    cin >> playerName;

    carpos = -1 + WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;//both starts with the same position

    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2); cout << "Car simulation Game";
    gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
    gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
    gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
    gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
    gotoxy(WIN_WIDTH + 2, 14); cout << "A Key - Left";
    gotoxy(WIN_WIDTH + 2, 15); cout << "D Key - Right";

    gotoxy(18, 5); cout << "Press any key to start";
    getch();
    gotoxy(18, 5); cout << "                      ";

    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 'a' || ch == 'A' || ch == 75) {
                if (carpos > 18)
                    carpos -= 4;
            }
            if (ch == 'd' || ch == 'D' || ch == 77) {
                if (carpos < 50)
                    carpos += 4;
            }
            if (ch == 27) {
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if (collision() == 1) {
            gameover();
            return;
        }
        Sleep(30);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if (enemyY[0] == 10)
            if (enemyFlag[1] == 0)
                enemyFlag[1] = 1;

        if (enemyFlag[0] == 1)
            enemyY[0] += 1;

        if (enemyFlag[1] == 1)
            enemyY[1] += 1;

        if (enemyY[0] > SCREEN_HEIGHT - 4) {
            resetEnemy(0);
            score++;
            updateScore();
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4) {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}

int main() {
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    do {
        system("cls");
        gotoxy(10, 5); cout << " -------------------------- ";
        gotoxy(10, 6); cout << " |        Car simulation Game        | ";
        gotoxy(10, 7); cout << " --------------------------";
        gotoxy(10, 9); cout << "1. Start Game";
        gotoxy(10, 10); cout << "2. Instructions";
        gotoxy(10, 11); cout << "3. Top Scores";
        gotoxy(10, 12); cout << "4. Quit";
        gotoxy(10, 14); cout << "Select option: ";
        char op = getche();

        if (op == '1') {
            play();
        } else if (op == '2') {
            instructions();
        } else if (op == '3') {
            displayTopScores();
        } else if (op == '4') {
            exit(0);
        }

    } while (1);

    return 0;
}
