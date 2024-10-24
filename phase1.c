#include <stdio.h>
#include <stdlib.h>
#include <time.h> //Needed for random number generation
#include <string.h> //In order to use strcmp()
#include <ctype.h> // included to provide functions for character handling 

#define GRID_SIZE 10

// Function prototypes
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses);
int askDifficulty();
void getPlayerNames(char player1[50], char player2[50]);
int chooseFirstPlayer();
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char* shipName, const char* playerName); // Function to clear the input buffer safely
void clearInputBuffer();
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col);
int checkWin(char board[GRID_SIZE][GRID_SIZE]);  

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
    placeShip(player1Board, 5, "Carrier", player1);
    placeShip(player1Board, 4, "Battleship", player1);
    placeShip(player1Board, 3, "Destroyer", player1);
    placeShip(player1Board, 2, "Submarine", player1);

    // Fleet setup for Player 2
    printf("\n%s, it's time to place your fleet!\n", player2);
    placeShip(player2Board, 5, "Carrier", player2);
    placeShip(player2Board, 4, "Battleship", player2);
    placeShip(player2Board, 3, "Destroyer", player2);
    placeShip(player2Board, 2, "Submarine", player2);

    // Turn-based play
    int gameOver = 0;
    int currentPlayer = firstPlayer;
    while (!gameOver) {
        if (currentPlayer == 0) {
            printf("\n%s's turn to fire!\n", player1);
            displayBoard(player2Board, 1);  // Show player 2's board (opponent's grid)

            // Fire command
            int row;
            char col;
            printf("Enter coordinates to fire (row column, e.g., 3 B): ");
            if (scanf("%d %c", &row, &col) != 2) {
                printf("Invalid input. Try again.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            col = toupper(col) - 'A';  // Convert column letter to index
            row--;  // Convert row to 0-based index

            if (fireAt(player2Board, row, col)) {
                if (checkWin(player2Board)) {
                    printf("\n%s wins!\n", player1);
                    gameOver = 1;
                }
            }
            currentPlayer = 1;  // Switch to player 2
        } else {
            printf("\n%s's turn to fire!\n", player2);
            displayBoard(player1Board, 1);  // Show player 1's board (opponent's grid)

            // Fire command
            int row;
            char col;
            printf("Enter coordinates to fire (row column, e.g., 3 B): ");
            if (scanf("%d %c", &row, &col) != 2) {
                printf("Invalid input. Try again.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            col = toupper(col) - 'A';  // Convert column letter to index
            row--;  // Convert row to 0-based index

            if (fireAt(player1Board, row, col)) {
                if (checkWin(player1Board)) {
                    printf("\n%s wins!\n", player2);
                    gameOver = 1;
                }
            }
            currentPlayer = 0;  // Switch to player 1
        }
    }

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
    if (scanf("%d", &difficulty) != 1 || (difficulty != 1 && difficulty != 2)) {
        printf("Invalid input. Defaulting to hard mode.\n");
        difficulty = 2;
    }
    clearInputBuffer();  // Clear input buffer after scanf
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
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char* shipName, const char* playerName) {
    int row;
    char col;  // Use char for column input
    char orientation[20];  // Increased buffer size to handle larger inputs

    while (1) {
        // Display player name before asking for the starting position
        printf("%s, placing your %s (size %d):\n", playerName, shipName, shipSize);
        printf("%s, enter starting position (row column, e.g., 3 B): ", playerName);
        if (scanf("%d %c", &row, &col) != 2) {
            printf("Invalid input. Try again.\n");
            clearInputBuffer();  // Clear input buffer
            continue;
        }
        clearInputBuffer();  // Clear input buffer

        col = toupper(col) - 'A';  // Convert column letter to index (0-9) and make it case-insensitive
        row--;  // Adjust row input (1-10) to array index (0-9)

        // Initialize validPlacement for each iteration
        int validPlacement = 1;

        // Check if row and col are within valid bounds
        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
            printf("Error: Invalid starting position for %s. Row or column out of bounds.\n", shipName);
            continue;  // Ask for input again if invalid
        }

        printf("Enter orientation (horizontal or vertical): ");
        scanf("%19s", orientation);  // Limit input to avoid buffer overflow
        clearInputBuffer();  // Clear input buffer

        // Convert orientation to lowercase to make it case-insensitive
        for (int i = 0; orientation[i]; i++) {
            orientation[i] = tolower(orientation[i]);
        }

        // Check if the ship can be placed within the grid boundaries
        if (strcmp(orientation, "horizontal") == 0) {
            // Horizontal: check if ship fits within the row
            if (col + shipSize > GRID_SIZE) {
                printf("Error: Ship placement exceeds grid boundaries horizontally. Try again.\n");
                validPlacement = 0;
            } else {
                // Check for overlapping ships
                for (int i = 0; i < shipSize; i++) {
                    if (board[row][col + i] != '~') {
                        printf("Error: Ship overlaps with another ship at row %d and column %c. Try again.\n", row + 1, 'A' + col + i);
                        validPlacement = 0;
                        break;
                    }
                }
            }
        } else if (strcmp(orientation, "vertical") == 0) {
            // Vertical: check if ship fits within the column
            if (row + shipSize > GRID_SIZE) {
                printf("Error: Ship placement exceeds grid boundaries vertically. Try again.\n");
                validPlacement = 0;
            } else {
                // Check for overlapping ships
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
                    // Ensure we are not exceeding the boundaries of the grid
                    if (col + i < GRID_SIZE) {
                        board[row][col + i] = shipName[0];  // Use the first letter of the ship name
                    }
                }
            } else {
                for (int i = 0; i < shipSize; i++) {
                    // Ensure we are not exceeding the boundaries of the grid
                    if (row + i < GRID_SIZE) {
                        board[row + i][col] = shipName[0];  // Use the first letter of the ship name
                    }
                }
            }
            displayBoard(board, 1);  // Show the updated board for debugging purposes
            break;  // Exit the loop when placement is valid
        }
    }
    return 1;
}

// Function to clear the input buffer safely
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to fire at a coordinate
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col) {
    if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE) {
        printf("Invalid coordinates. Try again.\n");
        return 0;
    }

    if (board[row][col] == '~') {
        printf("Miss!\n");
        board[row][col] = 'o';  // Mark miss
    } else if (board[row][col] == '*' || board[row][col] == 'o') {
        printf("Already fired at this location. Try again.\n");
        return 0;
    } else {
        printf("Hit!\n");
        board[row][col] = '*';  // Mark hit
    }

    return 1;
}

// Function to check if all ships have been sunk
int checkWin(char board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'o') {
                return 0;  // There are still ships left
            }
        }
    }
    return 1;  // All ships have been sunk
}