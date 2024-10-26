// Improvements to add:
// 1. everytime a player uses smoke, remind player how many smoke they have left
// 2. everytime a player sinks a ship, remind player how much smoke they have left
// 3. add condition in smoke if a player attempts to perform more than their allowed limit, they lose their turn.
// 3. fix delay in terminal (when smoke is used, it takes a bit more time than other moves to move to next player's turn)
// 4. fix terminal from clearing so fast after player is done placing all ships, so player has time to see final placement of their ships on board.
// 5. add Torpedo function

// easy mode: player loses turn if input invalid coordinates (done), hits and misses are shown on screen (done), player is warned when repeated coordinates are fired at and gets to change coordinates (done), smoke count tracked.
// hard mode: player loses turn if input invalid coordinates (done), only hits are displayed (done), player is not warned when firing at the same coordinate (done), player is not reminded how much smoke they have (idea in progress).

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //Needed for random number generation
#include <string.h> //In order to use strcmp()
#include <ctype.h>  // included to provide functions for character handling
#include <unistd.h> // For sleep function on Unix-based systems

#define GRID_SIZE 10

// Separate radar grids for each player to track smoked areas independently
char player1Radar[GRID_SIZE][GRID_SIZE];
char player2Radar[GRID_SIZE][GRID_SIZE];

// Function prototypes
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void initializeRadar(char radar[GRID_SIZE][GRID_SIZE]);                             // fills the radar grid with water to indicate that no areas have been discovered or smoked yet.
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses, int hideShips); // added parameter hideShips for game play
int askDifficulty();
void getPlayerNames(char player1[50], char player2[50]);
int chooseFirstPlayer(); // Randomly choose which player goes first
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char *shipName, const char *playerName, char shipInitial);
void clearInputBuffer();                                                                                                                       // Function to clear the input buffer safely
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *artilleryAllowed, int *artilleryUnlocked, int *shipsSunk, int showMisses); // Fire at a specific coordinate // Modified function signature
int radarSweep(char board[GRID_SIZE][GRID_SIZE], char radar[GRID_SIZE][GRID_SIZE], int row, int col);                                          // Function prototype for radar sweep with smokeBoard
int artilleryStrike(char board[GRID_SIZE][GRID_SIZE], int row, int col);                                                                       // Artillery strike

// Smoke Screen Functionality
void applySmokeScreen(char radar[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int *smokeUsed);
void clearScreen();

int checkWin(char board[GRID_SIZE][GRID_SIZE]);
const char *checkShipSunk(char board[GRID_SIZE][GRID_SIZE], char shipChar); // Returns the name of the ship if it is fully sunk, otherwise returns NULL.

int main()
{
    // Create boards for Player 1 and Player 2
    char player1Board[GRID_SIZE][GRID_SIZE];
    char player2Board[GRID_SIZE][GRID_SIZE];
    char player1[50], player2[50]; // Player names
    int player1RadarCount = 0;
    int player2RadarCount = 0;
    int player1ArtilleryAllowed = 0;
    int player2ArtilleryAllowed = 0;
    int player1ArtilleryUnlocked = 0;
    int player2ArtilleryUnlocked = 0;
    int player1ShipsSunk = 0, player2ShipsSunk = 0;
    int player1SmokeUsed = 0, player2SmokeUsed = 0;

    // Initialize game boards and radars
    initializeBoard(player1Board);
    initializeBoard(player2Board);
    initializeRadar(player1Radar);
    initializeRadar(player2Radar);

    // Welcome message and difficulty selection
    printf("Welcome to Battleship!\n\n");
    int showMisses = askDifficulty(); // Easy mode: showMisses == 1, Hard mode: showMisses == 0

    // Get player names
    getPlayerNames(player1, player2);

    // Randomly choose which player goes first
    int firstPlayer = chooseFirstPlayer();
    if (firstPlayer == 0)
    {
        printf("%s will go first!\n", player1);
    }
    else
    {
        printf("%s will go first!\n", player2);
    }

    // Player 1 places ships
    printf("\n%s, it's time to place your fleet!\n", player1);
    placeShip(player1Board, 5, "Carrier", player1, 'C');
    placeShip(player1Board, 4, "Battleship", player1, 'B');
    placeShip(player1Board, 3, "Destroyer", player1, 'D');
    placeShip(player1Board, 2, "Submarine", player1, 'S');
    clearScreen(); // for secrecy

    // Player 2 places ships
    printf("\n%s, it's time to place your fleet!\n", player2);
    placeShip(player2Board, 5, "Carrier", player2, 'C');
    placeShip(player2Board, 4, "Battleship", player2, 'B');
    placeShip(player2Board, 3, "Destroyer", player2, 'D');
    placeShip(player2Board, 2, "Submarine", player2, 'S');
    clearScreen();

    // Turn-based play
    int gameOver = 0;
    int currentPlayer = firstPlayer;

    // Main game loop
    while (!gameOver)
    {
        clearScreen();
        if (currentPlayer == 0)
        {
            // Player 1's turn
            printf("\n%s's turn to play!\n", player1);
            displayBoard(player2Board, showMisses, 1);
            printf("Available moves: Fire, Radar");

            if (player1ArtilleryAllowed)
                printf(", Artillery");
            if (player1ShipsSunk > 0)
                printf(", Smoke");
            printf("\n");

            int validMove = 0;
            while (!validMove)
            {
                char command[10];
                printf("Enter 'fire', 'radar', 'artillery', or 'smoke': ");
                scanf("%9s", command);
                clearInputBuffer();

                if (strcmp(command, "fire") == 0)
                {
                    // Fire command logic
                    int row;
                    char col;
                    printf("Enter coordinates to fire (row column, e.g., 3 B): ");

                    if (scanf("%d %c", &row, &col) == 2)
                    {
                        clearInputBuffer();
                        col = toupper(col) - 'A'; // Convert column letter to index
                        row--;                    // Convert row to 0-based index

                        // Check if coordinates are within bounds before calling fireAt
                        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
                        {
                            printf("Invalid coordinates. You lost your turn!\n");
                            validMove = 1; // End current player's turn due to invalid coordinates
                        }
                        else
                        {
                            // Fire at specified coordinates with mode-specific behavior
                            int fireResult = fireAt(player2Board, row, col, &player1ArtilleryAllowed, &player1ArtilleryUnlocked, &player1ShipsSunk, showMisses);
                            if (fireResult == 1 || (fireResult == -1 && showMisses == 0))
                            { // Continue in hard mode even if location was previously fired at
                                validMove = 1;
                                if (checkWin(player2Board))
                                {
                                    printf("\n%s wins!\n", player1);
                                    gameOver = 1;
                                }
                            }
                            else if (fireResult == -1 && showMisses == 1)
                            { // Only warn and re-prompt in easy mode
                                printf("Already fired at this location. Try again.\n");
                            }
                        }
                    }
                    else
                    {
                        printf("Invalid input. Try again.\n");
                        clearInputBuffer();
                    }
                }
                else if (strcmp(command, "radar") == 0)
                {
                    // Radar move for player 1
                    if (player1RadarCount >= 3)
                    {
                        printf("You have already used all 3 radar sweeps.\n");
                    }
                    else
                    {
                        int row;
                        char col;
                        printf("Enter top-left coordinates for radar sweep (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            radarSweep(player2Board, player2Radar, row, col);
                            player1RadarCount++;
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                }
                else if (strcmp(command, "artillery") == 0)
                {
                    // Artillery move for player 1
                    if (player1ArtilleryAllowed)
                    {
                        int row;
                        char col;
                        printf("Enter top-left coordinates for artillery strike (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            artilleryStrike(player2Board, row, col);
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                    else
                    {
                        printf("Artillery not unlocked yet!\n");
                        printf("Artillery is unlocked when you sink your opponent's ship.\n");
                    }
                }
                else if (strcmp(command, "smoke") == 0)
                {
                    // Smoke screen move for player 1
                    if (player1ShipsSunk == 0)
                    {
                        printf("Need to sink a ship to unlock smoke screen.\n");
                    }
                    else if (player1SmokeUsed >= player1ShipsSunk)
                    {
                        printf("You are only allowed one smoke screen per ship sunk.\n");
                    }
                    else
                    {
                        int row;
                        char col;
                        printf("Enter coordinates to apply smoke screen (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            applySmokeScreen(player1Radar, row, col, &player1ShipsSunk, &player1SmokeUsed);
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                }
                else
                {
                    printf("Invalid move. Try again.\n");
                }
            }
            sleep(2);
            currentPlayer = 1;
        }
        else
        {

            // Player 2's turn (similar logic as Player 1)
            printf("\n%s's turn to play!\n", player2);
            displayBoard(player1Board, showMisses, 1);
            printf("Available moves: Fire, Radar");
            if (player2ArtilleryAllowed)
                printf(", Artillery");
            if (player2ShipsSunk > 0)
                printf(", Smoke");
            printf("\n");

            int validMove = 0;
            while (!validMove)
            {

                char command[10];
                printf("Enter 'fire', 'radar', 'artillery', or 'smoke': ");
                scanf("%9s", command);
                clearInputBuffer();

                if (strcmp(command, "fire") == 0)
                {
                    // Fire command logic
                    int row;
                    char col;
                    printf("Enter coordinates to fire (row column, e.g., 3 B): ");
                    if (scanf("%d %c", &row, &col) == 2)
                    {
                        clearInputBuffer();
                        col = toupper(col) - 'A';
                        row--;

                        // Check if coordinates are within bounds before calling fireAt
                        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
                        {
                            printf("Invalid coordinates. You lost your turn!\n");
                            validMove = 1; // End current player's turn due to invalid coordinates
                        }
                        else
                        {
                            // Fire at specified coordinates with mode-specific behavior
                            int fireResult = fireAt(player1Board, row, col, &player2ArtilleryAllowed, &player2ArtilleryUnlocked, &player2ShipsSunk, showMisses);
                            if (fireResult == 1 || (fireResult == -1 && showMisses == 0))
                            { // Continue in hard mode even if location was previously fired at
                                validMove = 1;
                                if (checkWin(player1Board))
                                {
                                    printf("\n%s wins!\n", player2);
                                    gameOver = 1;
                                }
                            }
                            else if (fireResult == -1 && showMisses == 1)
                            { // Only warn and re-prompt in easy mode
                                printf("Already fired at this location. Try again.\n");
                            }
                        }
                    }
                    else
                    {
                        printf("Invalid input. Try again.\n");
                        clearInputBuffer();
                    }
                }
                else if (strcmp(command, "radar") == 0)
                {
                    // Radar move for player 2
                    if (player2RadarCount >= 3)
                    {
                        printf("You have already used all 3 radar sweeps.\n");
                    }
                    else
                    {
                        int row;
                        char col;
                        printf("Enter top-left coordinates for radar sweep (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            radarSweep(player1Board, player1Radar, row, col);
                            player2RadarCount++;
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                }
                else if (strcmp(command, "artillery") == 0)
                {
                    // Artillery move for player 2
                    if (player2ArtilleryAllowed)
                    {
                        int row;
                        char col;
                        printf("Enter top-left coordinates for artillery strike (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            artilleryStrike(player1Board, row, col);
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                    else
                    {
                        printf("Artillery not unlocked yet!\n");
                        printf("Artillery is unlocked when you sink your opponent's ship.\n");
                    }
                }
                else if (strcmp(command, "smoke") == 0)
                {
                    // Smoke screen move for player 2
                    if (player2ShipsSunk == 0)
                    {
                        printf("Need to sink a ship to unlock smoke screen.\n");
                    }
                    else if (player2SmokeUsed >= player2ShipsSunk)
                    {
                        printf("You are only allowed one smoke screen per ship sunk.\n");
                    }
                    else
                    {
                        int row;
                        char col;
                        printf("Enter coordinates to apply smoke screen (row column, e.g., 3 B): ");
                        if (scanf("%d %c", &row, &col) == 2)
                        {
                            clearInputBuffer();
                            col = toupper(col) - 'A';
                            row--;
                            applySmokeScreen(player2Radar, row, col, &player2ShipsSunk, &player2SmokeUsed);
                            validMove = 1;
                        }
                        else
                        {
                            printf("Invalid input. Try again.\n");
                            clearInputBuffer();
                        }
                    }
                }
                else
                {
                    printf("Invalid move. Try again.\n");
                }
            }
            sleep(2);
            currentPlayer = 0;
        }
    }

    return 0;
}

// Function to initialize the game board with water '~'
void initializeBoard(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            board[i][j] = '~'; // Initialize each cell with water symbol '~'
        }
    }
}

// Function to initialize the radar grid with no smoke
void initializeRadar(char radar[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            radar[i][j] = '~';
        }
    }
}

// Function to display the game board with row and column labels
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses, int hideShips)
{
    printf("  A B C D E F G H I J\n"); // Column labels
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%2d ", i + 1); // Row labels
        for (int j = 0; j < GRID_SIZE; j++)
        {
            // Hide ships from the opponent if hideShips is 1
            if (hideShips && (board[i][j] != '*' && board[i][j] != 'o'))
            {
                printf("~ "); // Display water where the ships are hidden
            }
            else if (board[i][j] == '*' || (showMisses && board[i][j] == 'o'))
            {
                printf("%c ", board[i][j]);
            }
            else if (board[i][j] == 'o' && !showMisses)
            {
                printf("~ "); // In hard mode, replace misses 'o' with water '~'
            }
            else
            {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to ask the user for the tracking difficulty
int askDifficulty()
{
    int difficulty;
    printf("Choose tracking difficulty level:\n");
    printf("1. Easy (Track hits and misses)\n");
    printf("2. Hard (Track only hits)\n");
    printf("Enter 1 or 2: ");
    if (scanf("%d", &difficulty) != 1 || (difficulty != 1 && difficulty != 2))
    {
        printf("Invalid input. Defaulting to hard mode.\n");
        difficulty = 2;
    }
    clearInputBuffer();               // Clear input buffer after scanf
    return (difficulty == 1) ? 1 : 0; // Return 1 for easy, 0 for hard
}

// Function to get player names
void getPlayerNames(char player1[50], char player2[50])
{
    printf("Enter Player 1's name: ");
    scanf("%s", player1);

    printf("Enter Player 2's name: ");
    scanf("%s", player2);
}

// Function to randomly choose which player goes first
int chooseFirstPlayer()
{
    srand(time(NULL)); // Seed the random number generator with the current time
    return rand() % 2; // Returns 0 for Player 1 or 1 for Player 2
}

// Function to place a ship on the board
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char *shipName, const char *playerName, char shipInitial)
{
    int row;
    char col;             // Use char for column input
    char orientation[20]; // Increased buffer size to handle larger inputs

    while (1)
    {
        // Display player name before asking for the starting position
        printf("%s, placing your %s (size %d):\n", playerName, shipName, shipSize);
        printf("%s, enter starting position (row column, e.g., 3 B): ", playerName);
        if (scanf("%d %c", &row, &col) != 2)
        {
            printf("Invalid input. Try again.\n");
            clearInputBuffer(); // Clear input buffer
            continue;
        }
        clearInputBuffer(); // Clear input buffer

        col = toupper(col) - 'A'; // Convert column letter to index (0-9) and make it case-insensitive
        row--;                    // Adjust row input (1-10) to array index (0-9)

        // Initialize validPlacement for each iteration
        int validPlacement = 1;

        // Check if row and col are within valid bounds
        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
        {
            printf("Error: Invalid starting position for %s. Row or column out of bounds.\n", shipName);
            continue; // Ask for input again if invalid
        }

        printf("Enter orientation (horizontal or vertical): ");
        scanf("%19s", orientation); // Limit input to avoid buffer overflow
        clearInputBuffer();         // Clear input buffer

        // Convert orientation to lowercase to make it case-insensitive
        for (int i = 0; orientation[i]; i++)
        {
            orientation[i] = tolower(orientation[i]);
        }

        // Check if the ship can be placed within the grid boundaries
        if (strcmp(orientation, "horizontal") == 0)
        {
            // Horizontal: check if ship fits within the row
            if (col + shipSize > GRID_SIZE)
            {
                printf("Error: Ship placement exceeds grid boundaries horizontally. Try again.\n");
                validPlacement = 0;
            }
            else
            {
                // Check for overlapping ships
                for (int i = 0; i < shipSize; i++)
                {
                    if (board[row][col + i] != '~')
                    {
                        printf("Error: Ship overlaps with another ship at row %d and column %c. Try again.\n", row + 1, 'A' + col + i);
                        validPlacement = 0;
                        break;
                    }
                }
            }
        }
        else if (strcmp(orientation, "vertical") == 0)
        {
            // Vertical: check if ship fits within the column
            if (row + shipSize > GRID_SIZE)
            {
                printf("Error: Ship placement exceeds grid boundaries vertically. Try again.\n");
                validPlacement = 0;
            }
            else
            {
                // Check for overlapping ships
                for (int i = 0; i < shipSize; i++)
                {
                    if (board[row + i][col] != '~')
                    {
                        printf("Error: Ship overlaps with another ship at row %d and column %c. Try again.\n", row + 1 + i, 'A' + col);
                        validPlacement = 0;
                        break;
                    }
                }
            }
        }
        else
        {
            printf("Invalid orientation. Try again.\n");
            continue;
        }

        // Place the ship if valid
        if (validPlacement)
        {
            printf("%s placed successfully!\n\n", shipName);
            if (strcmp(orientation, "horizontal") == 0)
            {
                for (int i = 0; i < shipSize; i++)
                {
                    board[row][col + i] = shipInitial;
                }
            }
            else
            {
                for (int i = 0; i < shipSize; i++)
                {
                    {
                        board[row + i][col] = shipInitial;
                    }
                }
            }
            displayBoard(board, 1, 0); // Show the updated board for debugging purposes
            break;                     // Exit the loop when placement is valid
        }
    }
    return 1;
}

// Function to clear the input buffer safely
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Function to fire at a coordinate
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *artilleryAllowed, int *artilleryUnlocked, int *shipsSunk, int showMisses)
{ // Added showMisses parameter
    if ((board[row][col] == '*' || board[row][col] == 'o') && showMisses == 1)
    {
        return -1; // Location already fired upon, only warn in easy mode
    }

    if (board[row][col] == '~')
    {
        printf("Miss!\n");
        board[row][col] = 'o';
    }
    else
    {
        printf("Hit!\n");
        char shipChar = board[row][col];
        board[row][col] = '*';

        const char *sunkShip = checkShipSunk(board, shipChar);
        if (sunkShip != NULL)
        {
            printf("You sunk the %s!\n", sunkShip);
            printf("Allowed 1 smoke screen.\n");

            if (!*artilleryUnlocked)
            {
                printf("Artillery unlocked!\n");
                *artilleryUnlocked = 1;
            }
            (*shipsSunk)++; // Increment sunk ship count
            *artilleryAllowed = 1;
        }
    }

    return 1; // Successful fire
}

// Function to check if a specific ship has been sunk and return the ship's name
const char *checkShipSunk(char board[GRID_SIZE][GRID_SIZE], char shipChar)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board[i][j] == shipChar)
            {
                return NULL;
            }
        }
    }

    switch (shipChar)
    {
    case 'C':
        return "Carrier";
    case 'B':
        return "Battleship";
    case 'D':
        return "Destroyer";
    case 'S':
        return "Submarine";
    default:
        return NULL;
    }
}

// Function to perform a radar sweep in a 2x2 area and reveal if any ships are found
int radarSweep(char board[GRID_SIZE][GRID_SIZE], char radar[GRID_SIZE][GRID_SIZE], int row, int col)
{
    int foundShip = 0; // To track if a ship is found in the 2x2 area

    for (int i = row; i < row + 2 && i < GRID_SIZE; i++)
    {
        for (int j = col; j < col + 2 && j < GRID_SIZE; j++)
        {
            if (radar[i][j] == 'S')
            {
                printf("Enemy ships not found!\n");
                return 0;
            }
            else if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'o')
            {
                foundShip = 1;
            }
        }
    }

    if (foundShip)
    {
        printf("Enemy ships found!\n");
    }
    else
    {
        printf("No enemy ships found.\n");
    }

    return foundShip; // Return whether a ship was found or not
}

// Function to apply a smoke screen on a 2x2 area with limit check
void applySmokeScreen(char radar[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int *smokeUsed)
{
    if (*smokeUsed >= *shipsSunk)
    {
        printf("You are only allowed one smoke screen per ship sunk.\n");
        return;
    }

    if (row >= 0 && row < GRID_SIZE - 1 && col >= 0 && col < GRID_SIZE - 1)
    {
        for (int i = row; i <= row + 1; i++)
        {
            for (int j = col; j <= col + 1; j++)
            {
                radar[i][j] = 'S'; // Mark the radar grid as smoked
            }
        }
        (*smokeUsed)++; // Increase smoke usage count
        printf("Smoke screen applied at %d%c\n", row + 1, 'A' + col);

        // Sleep to keep the message on screen briefly before clearing
        sleep(2);      // Wait 2 seconds for the player to see the message
        clearScreen(); // Clear the screen after a brief pause
    }
    else
    {
        printf("Invalid smoke screen coordinates. Try again.\n");
    }
}

// Function to clear the screen for secrecy
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // Clear screen on Windows
#else
    system("clear"); // Clear screen on macOS/Linux
#endif
    printf("\033[H\033[J"); // ANSI escape code to clear any remaining screen buffer
    fflush(stdout);         // Flush output buffer to ensure the screen is fully cleared
}

// Function to perform an artillery strike in a 2x2 area
int artilleryStrike(char board[GRID_SIZE][GRID_SIZE], int row, int col)
{
    char hitShips[4] = {'\0', '\0', '\0', '\0'}; // Array to store ship characters hit
    int index = 0;
    int alreadySunk[256] = {0}; // Array to keep track of ships already checked for sinking

    printf("Performing artillery strike at %d %c!\n", row + 1, col + 'A');

    // Hit all cells in the 2x2 area
    for (int i = row; i < row + 2 && i < GRID_SIZE; i++)
    {
        for (int j = col; j < col + 2 && j < GRID_SIZE; j++)
        {
            if (board[i][j] == '~')
            {
                printf("Miss at %d %c.\n", i + 1, j + 'A');
                board[i][j] = 'o'; // Mark miss
            }
            else if (board[i][j] != '*' && board[i][j] != 'o')
            {
                printf("Hit at %d %c!\n", i + 1, j + 'A');
                hitShips[index++] = board[i][j]; // Store the hit ship character
                board[i][j] = '*';               // Mark hit
            }
        }
    }

    // Check if any ship is fully sunk, ensuring each ship is checked only once
    for (int i = 0; i < index; i++)
    {
        if (!alreadySunk[(unsigned char)hitShips[i]])
        { // Check if the ship was already checked
            const char *sunkShip = checkShipSunk(board, hitShips[i]);
            if (sunkShip != NULL)
            {
                printf("You sunk the %s!\n", sunkShip);
                alreadySunk[(unsigned char)hitShips[i]] = 1; // Mark this ship as checked
            }
        }
    }

    return 1;
}

// Function to check if all ships have been sunk
int checkWin(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'o')
            {
                return 0; // There are still ships left
            }
        }
    }
    return 1; // All ships have been sunk
}
