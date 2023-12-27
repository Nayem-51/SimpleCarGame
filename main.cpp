#include<iostream>
#include<conio.h>
#include<dos.h>
#include<windows.h>
#include<time.h>

// Define constants for screen dimensions
#define SCREEN_WIDTH 90  // Total width of the screen
#define SCREEN_HEIGHT 29 // Total height of the screen
#define WIN_WIDTH 70     // Width of the game window within the screen

using namespace std;

// Global variables
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// Arrays to store enemy positions and flags
int enemyY[3];  // Vertical positions of the three enemies
int enemyX[3];  // Horizontal positions of the three enemies
int enemyFlag[3];  // Flags for each enemy (purpose not yet clear)

char car[4][4] = { ' ','+','+',' ', // Top row of the car
                  '+','+','+','+',        // Middle rows of the car
                  ' ','+','+',' ',
                  '+','+','+','+'};         // Bottom row of the car
int carpos = WIN_WIDTH/2;
int score = 0;

// Function to set the cursor position
void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// Function to set cursor visibility and size
void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 50;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

// Function to draw the game border
void drawBorder(){
    for(int i=0; i<SCREEN_HEIGHT; i++){
        for(int j=0; j<18; j++){                           // Draw the left and right borders using 18 asterisks each
            gotoxy(0+j, i);                                    // Move the cursor to the current position on the left border
            cout << "*";
            gotoxy(WIN_WIDTH-j, i);
            cout << "*";
        }
    }
    for(int i=0; i<SCREEN_HEIGHT; i++){            // Iterate through each row again to draw a vertical line at the right edge
        gotoxy(SCREEN_WIDTH, i);
        cout << "*";
    }
}

// Function to generate enemy at a specific index
void genEnemy(int ind){
    enemyX[ind] = 17 + rand() % (33);       // Range from 17 to 50
}

// Function to draw an enemy at a specified index if its flag is active
void drawEnemy(int ind) {
 // Check if the enemy's flag is true (indicating it should be drawn)
 if (enemyFlag[ind] == true) {
   // Draw the enemy's shape using character blocks
   gotoxy(enemyX[ind], enemyY[ind]);  // Move cursor to top-left position
   cout << "####";  // Print the top row of the enemy
   gotoxy(enemyX[ind], enemyY[ind] + 1);  // Move down one row
   cout << " ## ";  // Print the middle row
   gotoxy(enemyX[ind], enemyY[ind] + 2);  // Move down another row
   cout << "####";  // Print the bottom row
   gotoxy(enemyX[ind], enemyY[ind] + 3);  // Move down one more row
   cout << " ## ";  // Print the bottom protrusions
 }
}


void eraseEnemy(int ind){
	if( enemyFlag[ind]== true ){
		gotoxy(enemyX[ind] , enemyY[ind]); 		    cout<<"    ";
		gotoxy(enemyX[ind] , enemyY[ind]+1);		cout<<"    ";
		gotoxy(enemyX[ind] , enemyY[ind]+2);		cout<<"    ";
		gotoxy(enemyX[ind] , enemyY[ind]+3);		cout<<"    ";
		gotoxy(WIN_WIDTH -40, 0); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 5); cout<<"            ";
		gotoxy(WIN_WIDTH -40, 5); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 10); cout<<"            ";
		gotoxy(WIN_WIDTH -40, 10); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 15); cout<<"             ";
		gotoxy(WIN_WIDTH -40, 15); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 20); cout<<"             ";
		gotoxy(WIN_WIDTH -40, 20); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 25); cout<<"             ";
		gotoxy(WIN_WIDTH -40, 25); cout<<"     ||     ";
	//	gotoxy(WIN_WIDTH -40, 30); cout<<"             ";
	}
}

// Function to reset an enemy at a specified index
void resetEnemy(int ind) {
  // Erase the enemy from its current position
  eraseEnemy(ind);

  // Move the enemy back to the top of the screen
  enemyY[ind] = 1;

  // Generate a new random horizontal position for the enemy
  genEnemy(ind);
}

// Function to draw the car on the screen
void drawCar() {
  // Iterate through each row and column of the car's visual representation
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Move the cursor to the appropriate position on the screen
      gotoxy(j + carpos, i + 22);
      // Print the character at the current position in the car array
      cout << car[i][j];
    }
  }
}

// Function to erase the car from the screen
void eraseCar() {
  // Iterate through each row and column of the car's previous position
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Move the cursor to the appropriate position
      gotoxy(j + carpos, i + 22);
      // Print spaces to overwrite the car's visual representation
      cout << " ";
    }
  }
}

// Function to check for collision between the car and an enemy
int collision() {
  // Check if the bottom of the first enemy is close enough to the car's top
  if (enemyY[0] + 4 >= 23) {
    // Check if the enemy's horizontal position overlaps with the car's width
    if (enemyX[0] + 4 - carpos >= 0 && enemyX[0] + 4 - carpos < 9) {
      // Collision detected!
      return 1;
    }
  }
  // No collision detected
  return 0;
}

// Function to display the game over message
void gameover() {
  // Clear the screen
  system("cls");

  // Print the game over message with a centered title and border
  cout << endl;
  cout << "\t\t----------------------------" << endl;
  cout << "\t\t--------- Game Over --------" << endl;
  cout << "\t\t----------------------------" << endl;
  cout << "\t\tPress any Key to go back to menu.";

  // Wait for a key press to continue
  getch();
}



// Function to update and display the score
void updateScore(){
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Score: " << score << endl;
}

// Function to display game instructions
void instructions(){
    system("cls");
    cout << "Instructions";
    cout << "\n-----------------";
    cout << "\n Avoid car by moving left or right.";
    cout << "\n\n Press 'Arrow Keys' to move the Car";
    cout << "\n\n Press 'escape' to Exit";
    cout << "\n\n Press any Key to go back to the menu";
    getch();
}

// Function to start the game
void play(){
    carpos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2);
    cout << "Car Game";
    gotoxy(WIN_WIDTH + 6, 4);
    cout << "----------";
    gotoxy(WIN_WIDTH + 6, 6);
    cout << "----------";
    gotoxy(WIN_WIDTH + 7, 12);
    cout << "Control ";
    gotoxy(WIN_WIDTH + 7, 13);
    cout << "-------- ";
    gotoxy(WIN_WIDTH + 2, 14);
    cout << "A Key - Left";
    gotoxy(WIN_WIDTH + 2, 15);
    cout << "D Key - Left";
    gotoxy(WIN_WIDTH + 2, 15);
    cout << "Left Arrow";
    gotoxy(WIN_WIDTH + 2, 14);
    cout << "Right Ket";

    gotoxy(18, 5);
    cout << "Press any key to start";
    getch();
    gotoxy(18, 5);
    cout << "                      ";

    while(1){
        if(kbhit()){
            char ch = getch();
            if(ch == 'a' || ch == 'A' || ch == 75){  // 75 is the ASCII code of the left arrow
                if(carpos > 18)  // Ensure the car does not go out of bounds on the left
                    carpos -= 4;
            }
            if(ch == 'd' || ch == 'D' || ch == 77){  // 77 is the ASCII code of the right arrow
                if(carpos < 50)  // Ensure the car does not go out of bounds on the right
                    carpos += 4;
            }
            if(ch == 27){  // 27 is the ASCII code of the escape key
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if(collision() == 1){
            gameover();
            return;
        }
        Sleep(50);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if(enemyY[0] == 10)
            if(enemyFlag[1] == 0)
                enemyFlag[1] = 1;

        if(enemyFlag[0] == 1)
            enemyY[0] += 1;

        if(enemyFlag[1] == 1)
            enemyY[1] += 1;

        if(enemyY[0] > SCREEN_HEIGHT - 4){
            resetEnemy(0);
            score++;
            updateScore();
        }
        if(enemyY[1] > SCREEN_HEIGHT - 4){
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}
// Main function of the program
int main() {
 // Hide the cursor (optional for visual clarity)
 setcursor(0, 0);

 // Seed the random number generator for unpredictable gameplay
 srand((unsigned)time(NULL));

 // Main game loop
 do {
   // Clear the screen and display the main menu
   system("cls");
   gotoxy(10, 5);
   cout << " -------------------------- ";
   gotoxy(10, 6);
 gotoxy(10,6); cout<<"  |    Simple Car Game   | ";
   gotoxy(10, 7);
   cout << " --------------------------";
   gotoxy(10, 9);
   cout << "1. Start Game";
   gotoxy(10, 10);
   cout << "2. Instructions";
   gotoxy(10, 11);
   cout << "3. Exit ";
   gotoxy(10, 13);
   cout << "Select option: ";

   // Get player's menu choice
   char op = getche();

   // Process the chosen option
   if (op == '1') {
     // Start the game
     play();
   } else if (op == '2') {
     // Display game instructions
     instructions();
   } else if (op == '3') {
     // Exit the game
     exit(0);
   }

 } while (1);  // Repeat the menu loop indefinitely

 // Indicate successful program termination (not reached due to infinite loop)
 return 0;
}
