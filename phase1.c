#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Needed for random number generation
#include <string.h> //In order to use strcmp()

#define GRID_SIZE 10

// Function prototypes
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses);
int askDifficulty();
void getPlayerNames(char player1[50], char player2[50]);
int chooseFirstPlayer();
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char* shipName);


int main() {
    char player1Board[GRID_SIZE][GRID_SIZE];
    char player2Board[GRID_SIZE][GRID_SIZE];
    char player1[50], player2[50];

    // Initialize both players' boards
    initializeBoard(player1Board);
    initializeBoard(player2Board);
    
    // Welcome statement
    printf("Welcome to Battleship!\n\n");
    
    // Ask for tracking difficulty
    int showMisses = askDifficulty();

    // Get player names
    getPlayerNames(player1, player2);

    // Randomly choose which player goes first
    int firstPlayer = chooseFirstPlayer();
    if (firstPlayer == 0) {
        printf("%s will go first!\n", player1);
    } else {
        printf("%s will go first!\n", player2);
    }

    //Display both boards initially
    //printf("%s's board:\n", player1);
    //displayBoard(player1Board, showMisses);
    
    //printf("%s's board:\n", player2);
    //displayBoard(player2Board, showMisses);

    // Fleet setup for Player 1
    printf("\n%s, it's time to place your fleet!\n", player1);
    placeShip(player1Board, 5, "Carrier");
    placeShip(player1Board, 4, "Battleship");
    placeShip(player1Board, 3, "Destroyer");
    placeShip(player1Board, 2, "Submarine");
    
    // Fleet setup for Player 2
    printf("\n%s, it's time to place your fleet!\n", player2);
    placeShip(player2Board, 5, "Carrier");
    placeShip(player2Board, 4, "Battleship");
    placeShip(player2Board, 3, "Destroyer");
    placeShip(player2Board, 2, "Submarine");

    return 0;
}

// Function to initialize the game board with water '~'
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            board[i][j] = '~';  // Initialize each cell with water symbol '~'
        }
    }
}

// Function to display the game board with row and column labels
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses) {
    printf("  A B C D E F G H I J\n");  // Column labels
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%2d ", i + 1);  // Row labels
        for (int j = 0; j < GRID_SIZE; j++) {
            // If showMisses is 1, show both hits (*) and misses (o), otherwise only show hits
            if (board[i][j] == '*' || (showMisses && board[i][j] == 'o')) {
                printf("%c ", board[i][j]);
            } else if (board[i][j] == 'o' && !showMisses) {
                printf("~ ");  // In hard mode, replace misses 'o' with water '~'
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to ask the user for the tracking difficulty
int askDifficulty() {
    int difficulty;
    printf("Choose tracking difficulty level:\n");
    printf("1. Easy (Track hits and misses)\n");
    printf("2. Hard (Track only hits)\n");
    printf("Enter 1 or 2: ");
    scanf("%d", &difficulty);
    return (difficulty == 1) ? 1 : 0;  // Return 1 for easy, 0 for hard
}

// Function to get player names
void getPlayerNames(char player1[50], char player2[50]) {
    printf("Enter Player 1's name: ");
    scanf("%s", player1);
    
    printf("Enter Player 2's name: ");
    scanf("%s", player2);
}

// Function to randomly choose which player goes first
int chooseFirstPlayer() {
    srand(time(NULL));  // Seed the random number generator with the current time
    return rand() % 2;  // Returns 0 for Player 1 or 1 for Player 2
}

// Function to place a ship on the board
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char* shipName) {
    int row;
    char col;  // Use char for column input

    while (1) { //infinite loop 
        // Ask for the starting position and orientation
        printf("Placing your %s (size %d):\n", shipName, shipSize);
        printf("Enter starting position (row column, e.g., 3 B): ");
        scanf("%d %c", &row, &col);
        while ((getchar()) != '\n');  // Clear input buffer

        col = col - 'A';  // Convert column letter to index (0-9)
        row--;  // Adjust row input (1-10) to array index (0-9)

        // Initialize validPlacement for each iteration
        int validPlacement = 1;

        // Check if row and col are within valid bounds
        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
            printf("Error: Invalid starting position for %s. Row or column out of bounds.\n", shipName);
            continue;  // Ask for input again if invalid
        }

        printf("Enter orientation (horizontal or vertical): ");
        char orientation[10];  // Used to store the user's input for orientation
        scanf("%s", orientation);
        while ((getchar()) != '\n');  // Clear input buffer

        // Check if the ship can be placed within the grid boundaries
        if (strcmp(orientation, "horizontal") == 0) {
            if (col + shipSize > GRID_SIZE) {
                printf("Error: Ship placement exceeds grid boundaries horizontally. Try again.\n");
                validPlacement = 0;  // If the ship exceeds the grid size, invalidate the placement
            } else {
                // Check if the ship overlaps with another ship
                for (int i = 0; i < shipSize; i++) {
                    if (board[row][col + i] != '~') {
                        printf("Error: Ship overlaps with another ship at row %d and column %c. Try again.\n", row + 1, 'A' + col + i);
                        validPlacement = 0;
                        break;
                    }
                }
            }
        } else if (strcmp(orientation, "vertical") == 0) {
            if (row + shipSize > GRID_SIZE) {
                printf("Error: Ship placement exceeds grid boundaries vertically. Try again.\n");
                validPlacement = 0;  // If the ship exceeds the grid size, invalidate the placement
            } else {
                // Check if the ship overlaps with another ship
                for (int i = 0; i < shipSize; i++) {
                    if (board[row + i][col] != '~') {
                        printf("Error: Ship overlaps with another ship at row %d and column %c. Try again.\n", row + 1 + i, 'A' + col);
                        validPlacement = 0;
                        break;
                    }
                }
            }
        } else {
            printf("Invalid orientation. Try again.\n");
            continue;
        }

        // Place the ship if valid
        if (validPlacement) {
            printf("%s placed successfully!\n\n", shipName);
            if (strcmp(orientation, "horizontal") == 0) {
                for (int i = 0; i < shipSize; i++) {
                    board[row][col + i] = shipName[0];  // Use the first letter of the ship name
                }
            } else {
                for (int i = 0; i < shipSize; i++) {
                    board[row + i][col] = shipName[0];  // Use the first letter of the ship name
                }
            }
            displayBoard(board, 1);  // Show the updated board for debugging purposes
            break;  // Exit the loop when placement is valid
        }
    }
    return 1;
}


