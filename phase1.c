#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 10

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

int main() {
    char player1Board[GRID_SIZE][GRID_SIZE];
    char player2Board[GRID_SIZE][GRID_SIZE];
    
    // Initialize both players' boards
    initializeBoard(player1Board);
    initializeBoard(player2Board);
    
    // Welcome statement
    printf("Welcome to Battleship!\n\n");
    
    // Ask for tracking difficulty
    int showMisses = askDifficulty();
    
    // Display both boards initially
    printf("Player 1's board:\n");
    displayBoard(player1Board, showMisses);
    
    printf("Player 2's board:\n");
    displayBoard(player2Board, showMisses);

    return 0;
}
