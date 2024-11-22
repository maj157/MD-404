#define GRID_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

int player1SmokeEffectGrid[GRID_SIZE][GRID_SIZE];
int player2SmokeEffectGrid[GRID_SIZE][GRID_SIZE];

char player1Radar[GRID_SIZE][GRID_SIZE];
char player2Radar[GRID_SIZE][GRID_SIZE];

int easyBotShots[GRID_SIZE][GRID_SIZE];
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Initialization and Setup
void initializeGame(char player1Board[GRID_SIZE][GRID_SIZE], char botBoard[GRID_SIZE][GRID_SIZE],
                    char player1Radar[GRID_SIZE][GRID_SIZE], char player2Radar[GRID_SIZE][GRID_SIZE]);
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void initializeRadar(char radar[GRID_SIZE][GRID_SIZE]);
void initializeSmokeEffectGrid();
int askDifficulty();
void clearScreen();
void clearInputBuffer();

// Game Introduction
void placeFleet(char board[GRID_SIZE][GRID_SIZE], const char *playerName);
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char *shipName, const char *playerName, char shipInitial);

void easyBotPlaceShips(char board[GRID_SIZE][GRID_SIZE]);
void easyBotMakeMove(char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver);
int getUnvisitedNeighbors(int row, int col, int neighbors[4][2]);

// Gameplay
int executePlayerCommand(int player, char playerName[50], char opponentBoard[GRID_SIZE][GRID_SIZE], char playerRadar[GRID_SIZE][GRID_SIZE],
                         int *shipsSunk, int *smokeUsed, int *radarCount, int showMisses, int *gameOver);
void displayTurnInfo(const char *playerName, char opponentBoard[GRID_SIZE][GRID_SIZE], int showMisses, int shipsSunk);
int getCommandCode(const char *command);
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses, int hideShips);

// Moves
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int showMisses);
int radarSweep(char board[GRID_SIZE][GRID_SIZE], char radar[GRID_SIZE][GRID_SIZE], int row, int col, int player);
void applySmokeScreen(char radar[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int *smokeUsed, int *lostTurn, int player);
void displaySmokeCount(int showMisses, int currentPlayer, int player1ShipsSunk, int botShipsSunk, int player1SmokeUsed, int player2SmokeUsed);
int artilleryStrike(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk);
void torpedoAttack(char board[GRID_SIZE][GRID_SIZE], char target);
const char *checkShipSunk(char board[GRID_SIZE][GRID_SIZE], char shipChar);

int checkWin(char board[GRID_SIZE][GRID_SIZE]);

int main()
{
    // Create boards for Player 1 and SampleBot
    char player1Board[GRID_SIZE][GRID_SIZE];
    char botBoard[GRID_SIZE][GRID_SIZE];
    char player1[50] = "Player 1";
    int player1RadarCount = 0;
    int botShipsSunk = 0, player1ShipsSunk = 0;
    int player1SmokeUsed = 0;

    srand(time(NULL)); // Seed the random number generator

    // Initialize game boards and radars
    initializeGame(player1Board, botBoard, player1Radar, player2Radar);
    initializeSmokeEffectGrid(); // Initialize the smoke effect grid at the start of the game

    int showMisses = askDifficulty(); // Easy mode: showMisses == 1, Hard mode: showMisses == 0

    // Player 1 places ships
    placeFleet(player1Board, player1);

    // SampleBot places ships
    printf("SampleBot is placing its fleet...\n");
    easyBotPlaceShips(botBoard);

    // Randomly choose which player goes first
    int firstPlayer = rand() % 2;

    // Turn-based play
    int gameOver = 0;
    int currentPlayer = firstPlayer;

    // Main game loop
    while (!gameOver)
    {
        clearScreen();

        // Display the number of ships each player has sunk
        printf("Player 1 (%s) - Ships sunk: %d\n", player1, player1ShipsSunk);
        printf("easyBot - Ships sunk: %d\n\n", botShipsSunk);

        // Display remaining smoke count
        displaySmokeCount(showMisses, currentPlayer, player1ShipsSunk, botShipsSunk, player1SmokeUsed, 0);

        if (currentPlayer == 0)
        {
            // Player 1's
            printf("\nYour current grid:\n");
            displayBoard(player1Board, showMisses, 0);

            displayTurnInfo(player1, botBoard, showMisses, player1ShipsSunk);

            int validMove = 0;
            while (!validMove)
            {
                validMove = executePlayerCommand(1, player1, botBoard, player1Radar, &player1ShipsSunk,
                                                 &player1SmokeUsed, &player1RadarCount, showMisses, &gameOver);
            }

            if (gameOver)
                break;
            sleep(2);
            currentPlayer = 1;
        }
        else
        {
            
            printf("easyBot's turn to play...\n");
            easyBotMakeMove(player1Board, &botShipsSunk, &gameOver);

            if (gameOver)
                break;
            sleep(2);
            currentPlayer = 0;
        }
    }

    return 0;
}

void initializeGame(char player1Board[GRID_SIZE][GRID_SIZE], char botBoard[GRID_SIZE][GRID_SIZE],
                    char player1Radar[GRID_SIZE][GRID_SIZE], char player2Radar[GRID_SIZE][GRID_SIZE])
{
    // Initialize Player 1's and SampleBot's boards with water
    initializeBoard(player1Board);
    initializeBoard(botBoard);

    // Initialize radars for Player 1 and SampleBot
    initializeRadar(player1Radar);
    initializeRadar(player2Radar);

    // Initialize easyBot's shot tracking grid
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            easyBotShots[i][j] = 0; // Unvisited
        }
    }
}

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

void initializeSmokeEffectGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            player1SmokeEffectGrid[i][j] = 0; // Clear all smoke effects initially for Player 1
            player2SmokeEffectGrid[i][j] = 0; // Clear all smoke effects initially for SampleBot
        }
    }
}

int askDifficulty()
{
    printf("Welcome to Battleship!\n\n");
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

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void placeFleet(char board[GRID_SIZE][GRID_SIZE], const char *playerName)
{
    printf("\n%s, it's time to place your fleet!\n", playerName);
    placeShip(board, 5, "Carrier", playerName, 'C');
    placeShip(board, 4, "Battleship", playerName, 'B');
    placeShip(board, 3, "Destroyer", playerName, 'D');
    placeShip(board, 2, "Submarine", playerName, 'S');
    sleep(2);      // Allow time to view final ship placement
    clearScreen(); // for secrecy
}

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
                    board[row + i][col] = shipInitial;
                }
            }
            displayBoard(board, 1, 0); // Show the updated board for debugging purposes
            break;                     // Exit the loop when placement is valid
        }
    }
    return 1;
}

void easyBotPlaceShips(char board[GRID_SIZE][GRID_SIZE])
{
    const int shipSizes[] = {5, 4, 3, 2};
    const char shipChars[] = {'C', 'B', 'D', 'S'};

    for (int i = 0; i < 4; i++) // Iterate through the ships
    {
        int placed = 0;
        while (!placed)
        {
            int row = rand() % GRID_SIZE;
            int col = rand() % GRID_SIZE;
            int horizontal = rand() % 2; // 0 = vertical, 1 = horizontal
            placed = 1;

            // Check placement boundaries and overlaps
            if (horizontal)
            {
                if (col + shipSizes[i] > GRID_SIZE)
                {
                    placed = 0;
                }
                else
                {
                    for (int j = 0; j < shipSizes[i]; j++)
                    {
                        if (board[row][col + j] != '~')
                        {
                            placed = 0;
                            break;
                        }
                    }
                    if (placed)
                    {
                        for (int j = 0; j < shipSizes[i]; j++)
                        {
                            board[row][col + j] = shipChars[i];
                        }
                    }
                }
            }
            else
            {
                if (row + shipSizes[i] > GRID_SIZE)
                {
                    placed = 0;
                }
                else
                {
                    for (int j = 0; j < shipSizes[i]; j++)
                    {
                        if (board[row + j][col] != '~')
                        {
                            placed = 0;
                            break;
                        }
                    }
                    if (placed)
                    {
                        for (int j = 0; j < shipSizes[i]; j++)
                        {
                            board[row + j][col] = shipChars[i];
                        }
                    }
                }
            }
        }
    }
}

int getUnvisitedNeighbors(int row, int col, int neighbors[4][2]) 
{
    int count = 0;
    for (int i = 0; i < 4; i++) 
    {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE && easyBotShots[newRow][newCol] == 0) 
        {
            neighbors[count][0] = newRow;
            neighbors[count][1] = newCol;
            count++;
        }
    }
    return count;
}

void easyBotMakeMove(char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver)
{
    static int lastHitRow = -1, lastHitCol = -1; // Track the last successful hit
    static int artilleryUnlocked = 0;           // Flag to track artillery availability
    int neighbors[4][2];
    int foundTarget = 0;

    // Check for unvisited neighbors around the last hit
    if (lastHitRow != -1 && lastHitCol != -1)
    {
        int neighborCount = getUnvisitedNeighbors(lastHitRow, lastHitCol, neighbors);
        if (neighborCount > 0)
        {
            int targetIndex = rand() % neighborCount;
            int row = neighbors[targetIndex][0];
            int col = neighbors[targetIndex][1];

            if (artilleryUnlocked)
            {
                printf("easyBot uses artillery at %d%c\n", row + 1, 'A' + col);
                artilleryStrike(opponentBoard, row, col, shipsSunk);
            }
            else
            {
                printf("easyBot fires at %d%c\n", row + 1, 'A' + col);
                if (opponentBoard[row][col] != '~')
                {
                    printf("Hit!\n");
                    char shipChar = opponentBoard[row][col];
                    opponentBoard[row][col] = '*';
                    lastHitRow = row;
                    lastHitCol = col;

                    // Check if a ship has been sunk
                    const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                    if (sunkShip != NULL)
                    {
                        printf("easyBot sunk the %s!\n", sunkShip);
                        (*shipsSunk)++;
                        artilleryUnlocked = 1; // Unlock artillery after sinking a ship
                    }
                }
                else
                {
                    printf("Miss!\n");
                    opponentBoard[row][col] = 'o';
                    lastHitRow = -1;
                    lastHitCol = -1;
                }
            }

            // Check if the game is won
            if (checkWin(opponentBoard))
            {
                printf("easyBot wins!\n");
                *gameOver = 1;
            }
            easyBotShots[row][col] = 1;
            return;
        }
    }

    // If no valid neighbors, use the checkerboard pattern (improved search pattern)
    for (int i = 0; i < GRID_SIZE && !foundTarget; i++)
    {
        for (int j = (i % 2); j < GRID_SIZE; j += 2) // Checkerboard logic
        {
            if (easyBotShots[i][j] == 0)
            {
                if (artilleryUnlocked)
                {
                    printf("easyBot uses artillery at %d%c\n", i + 1, 'A' + j);
                    artilleryStrike(opponentBoard, i, j, shipsSunk);
                }
                else
                {
                    printf("easyBot fires at %d%c\n", i + 1, 'A' + j);
                    if (opponentBoard[i][j] != '~')
                    {
                        printf("Hit!\n");
                        char shipChar = opponentBoard[i][j];
                        opponentBoard[i][j] = '*';
                        lastHitRow = i;
                        lastHitCol = j;

                        // Check if a ship has been sunk
                        const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                        if (sunkShip != NULL)
                        {
                            printf("easyBot sunk the %s!\n", sunkShip);
                            (*shipsSunk)++;
                            artilleryUnlocked = 1; // Unlock artillery after sinking a ship
                        }
                    }
                    else
                    {
                        printf("Miss!\n");
                        opponentBoard[i][j] = 'o';
                    }
                }

                // Check if the game is won
                if (checkWin(opponentBoard))
                {
                    printf("easyBot wins!\n");
                    *gameOver = 1;
                }
                easyBotShots[i][j] = 1;
                return;
            }
        }
    }

    // If still no valid move, fire at any random unvisited cell
    int row, col;
    do
    {
        row = rand() % GRID_SIZE;
        col = rand() % GRID_SIZE;
    } while (easyBotShots[row][col] != 0);

    if (artilleryUnlocked)
    {
        printf("easyBot uses artillery at %d%c\n", row + 1, 'A' + col);
        artilleryStrike(opponentBoard, row, col, shipsSunk);
    }
    else
    {
        printf("easyBot fires at %d%c\n", row + 1, 'A' + col);
        if (opponentBoard[row][col] != '~')
        {
            printf("Hit!\n");
            char shipChar = opponentBoard[row][col];
            opponentBoard[row][col] = '*';
            lastHitRow = row;
            lastHitCol = col;

            // Check if a ship has been sunk
            const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
            if (sunkShip != NULL)
            {
                printf("easyBot sunk the %s!\n", sunkShip);
                (*shipsSunk)++;
                artilleryUnlocked = 1; // Unlock artillery after sinking a ship
            }
        }
        else
        {
            printf("Miss!\n");
            opponentBoard[row][col] = 'o';
        }
    }

    // Check if the game is won
    if (checkWin(opponentBoard))
    {
        printf("easyBot wins!\n");
        *gameOver = 1;
    }
    easyBotShots[row][col] = 1;
}

int executePlayerCommand(int player, char playerName[50], char opponentBoard[GRID_SIZE][GRID_SIZE],
                         char playerRadar[GRID_SIZE][GRID_SIZE],
                         int *shipsSunk, int *smokeUsed, int *radarCount, int showMisses, int *gameOver)
{
    int validMove = 0;
    char command[10];
    printf("Enter 'fire', 'radar', 'artillery', 'torpedo', or 'smoke': ");
    scanf("%9s", command);
    clearInputBuffer();

    int lostTurn = 0; // Reset lostTurn flag before each action
    int commandCode = getCommandCode(command);

    switch (commandCode)
    {
    case 1: // Fire
    {
        int row;
        char col;
        printf("Enter coordinates to fire (row column, e.g., 3 B): ");

        if (scanf("%d %c", &row, &col) == 2)
        {
            clearInputBuffer();
            col = toupper(col) - 'A';
            row--;

            if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
            {
                printf("Invalid coordinates. You lost your turn!\n");
                validMove = 1;
            }
            else
            {
                int fireResult = fireAt(opponentBoard, row, col, shipsSunk, showMisses);
                if (fireResult == 1 || (fireResult == -1 && showMisses == 0))
                {
                    validMove = 1;
                    if (checkWin(opponentBoard))
                    {
                        printf("\n%s wins!\n", playerName);
                        *gameOver = 1;
                    }
                }
                else if (fireResult == -1 && showMisses == 1)
                {
                    printf("Already fired at this location. Try again.\n");
                }
            }
        }
        else
        {
            printf("Invalid input. Try again.\n");
            clearInputBuffer();
        }
        break;
    }
    case 2: // Torpedo
    {
        if (*shipsSunk >= 3)
        {
            char target;
            printf("Enter a column (A-J) or row (1-10) for torpedo attack: ");
            scanf(" %c", &target);
            clearInputBuffer();

            if ((target >= 'A' && target <= 'J') || (target >= '1' && target <= '9') || target == '0')
            {
                torpedoAttack(opponentBoard, target);
                if (checkWin(opponentBoard))
                {
                    printf("\n%s wins!\n", playerName);
                    *gameOver = 1;
                }
                validMove = 1;
            }
            else
            {
                printf("Invalid coordinates. You lost your turn!\n");
                validMove = 1;
            }
        }
        else
        {
            printf("Torpedo not unlocked. You need to sink 3 ships to unlock it.\n");
        }
        break;
    }
    case 3: // Radar
    {
        if (*radarCount >= 3)
        {
            printf("You have already used all 3 radar sweeps and lost your turn.\n");
            validMove = 1; // Turn is lost if all radar sweeps are used
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

                if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
                {
                    printf("Invalid coordinates. You lost your turn!\n");
                    validMove = 1;
                }
                else
                {
                    radarSweep(opponentBoard, playerRadar, row, col, player);
                    (*radarCount)++;
                    validMove = 1;
                }
            }
            else
            {
                printf("Invalid input. Try again.\n");
                clearInputBuffer();
            }
        }
        break;
    }
    case 4: // Artillery
    {
        if (*shipsSunk >= 1)
        {
            int row;
            char col;
            printf("Enter top-left coordinates for artillery strike (row column, e.g., 3 B): ");
            if (scanf("%d %c", &row, &col) == 2)
            {
                clearInputBuffer();
                col = toupper(col) - 'A';
                row--;

                if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
                {
                    printf("Invalid coordinates. You lost your turn!\n");
                    validMove = 1;
                }
                else
                {
                    artilleryStrike(opponentBoard, row, col, shipsSunk);
                    if (checkWin(opponentBoard))
                    {
                        printf("\n%s wins!\n", playerName);
                        *gameOver = 1;
                    }
                    validMove = 1;
                }
            }
            else
            {
                printf("Invalid input. Try again.\n");
                clearInputBuffer();
            }
        }
        else
        {
            printf("Artillery not unlocked yet! Artillery is unlocked when you sink your opponent's ship.\n");
        }
        break;
    }
    case 5: // Smoke
    {
        if (*shipsSunk == 0)
        {
            printf("Need to sink a ship to unlock smoke screen.\n");
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

                if (row < 0 || row >= GRID_SIZE - 1 || col < 0 || col >= GRID_SIZE - 1)
                {
                    printf("Invalid coordinates. You lost your turn!\n");
                    validMove = 1;
                }
                else
                {
                    applySmokeScreen(playerRadar, row, col, shipsSunk, smokeUsed, &lostTurn, player);
                    validMove = 1;
                }
            }
            else
            {
                printf("Invalid input. Try again.\n");
                clearInputBuffer();
            }
        }
        break;
    }
    default:
        printf("Invalid move. Try again.\n");
        break;
    }
    return validMove;
}

void displayTurnInfo(const char *playerName, char opponentBoard[GRID_SIZE][GRID_SIZE], int showMisses, int shipsSunk)
{
    printf("\n%s's turn to play!\n", playerName);

    // Display the opponent's board for Player 1 (hidden ships)
    if (strcmp(playerName, "SampleBot") != 0)
    {
        displayBoard(opponentBoard, showMisses, 1);
    }

    // Display available moves for Player 1
    if (strcmp(playerName, "SampleBot") != 0)
    {
        printf("Available moves: Fire, Radar");
        if (shipsSunk >= 3)
            printf(", Torpedo");
        if (shipsSunk >= 1)
            printf(", Artillery");
        if (shipsSunk > 0)
            printf(", Smoke");
        printf("\n");
    }
}

int getCommandCode(const char *command)
{
    if (strcmp(command, "fire") == 0)
        return 1;
    if (strcmp(command, "torpedo") == 0)
        return 2;
    if (strcmp(command, "radar") == 0)
        return 3;
    if (strcmp(command, "artillery") == 0)
        return 4;
    if (strcmp(command, "smoke") == 0)
        return 5;
    return 0; // Invalid command
}

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

int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int showMisses)
{
    if ((board[row][col] == '*' || board[row][col] == 'o') && showMisses == 1)
    {
        return -1; // Location already fired upon, only warn in easy mode
    }

    if (board[row][col] == '~')
    {
        printf("Miss!\n");
        board[row][col] = 'o'; // Mark as miss
    }
    else
    {
        printf("Hit!\n");
        char shipChar = board[row][col];
        board[row][col] = '*'; // Mark as hit

        const char *sunkShip = checkShipSunk(board, shipChar);
        if (sunkShip != NULL)
        {
            printf("You sunk the %s!\n", sunkShip);
            (*shipsSunk)++;

            // Notify about unlocked special moves
            if (*shipsSunk >= 1)
                printf("Artillery unlocked!\n");
            if (*shipsSunk >= 3)
                printf("Torpedo unlocked!\n");
        }
    }

    return 1; // Successful fire
}

int radarSweep(char board[GRID_SIZE][GRID_SIZE], char radar[GRID_SIZE][GRID_SIZE], int row, int col, int player)
{
    int foundShip = 0;     // To track if a ship is found in the 2x2 area
    int smokeDetected = 0; // To track if any cell in the 2x2 area is smoked

    // Select the opponent's smoke effect grid based on the current player
    int(*smokeEffectGrid)[GRID_SIZE] = (player == 1) ? player2SmokeEffectGrid : player1SmokeEffectGrid;

    for (int i = row; i < row + 2 && i < GRID_SIZE; i++)
    {
        for (int j = col; j < col + 2 && j < GRID_SIZE; j++)
        {
            // Check if the cell is currently smoked on the opponent's radar
            if (smokeEffectGrid[i][j] == 1)
            {
                smokeDetected = 1;
            }
            else if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'o')
            {
                foundShip = 1; // Ship detected in non-smoked area
            }
        }
    }

    if (smokeDetected)
    {
        // Clear the smoke effect in the 2x2 area after it blocks this radar sweep for this player
        for (int i = row; i < row + 2 && i < GRID_SIZE; i++)
        {
            for (int j = col; j < col + 2 && j < GRID_SIZE; j++)
            {
                smokeEffectGrid[i][j] = 0; // Remove smoke effect for future sweeps
            }
        }
        printf("No enemy ships found.\n");
        return 0; // Return immediately due to smoke detection
    }

    // Print the result based on whether a ship was found
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

void applySmokeScreen(char opponentRadar[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int *smokeUsed, int *lostTurn, int player)
{
    if (*smokeUsed >= *shipsSunk)
    {
        printf("You are only allowed one smoke screen per ship sunk.\n");
        printf("You have lost your turn!\n");
        *lostTurn = 1;
        return;
    }

    // Determine which player's smoke effect grid to use based on the current player
    int(*smokeEffectGrid)[GRID_SIZE] = (player == 1) ? player1SmokeEffectGrid : player2SmokeEffectGrid;

    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
    {
        for (int i = row; i <= row + 1 && i < GRID_SIZE; i++)
        {
            for (int j = col; j <= col + 1 && j < GRID_SIZE; j++)
            {
                smokeEffectGrid[i][j] = 1; // Temporarily mark the area as smoked on the opponent's radar grid
            }
        }
        (*smokeUsed)++; // Increase smoke usage count
        printf("Smoke screen applied at %d%c\n", row + 1, 'A' + col);
    }
    else
    {
        printf("Invalid smoke screen coordinates. Try again.\n");
    }
}

void displaySmokeCount(int showMisses, int currentPlayer, int player1ShipsSunk, int player2ShipsSunk, int player1SmokeUsed, int player2SmokeUsed)
{
    if (showMisses == 1)
    { // Easy mode
        int remainingSmoke = (currentPlayer == 0)
                                 ? player1ShipsSunk - player1SmokeUsed
                                 : player2ShipsSunk - player2SmokeUsed;

        printf("Player %d, you have %d smoke(s) left.\n", currentPlayer + 1, remainingSmoke);
    }
    else
    { // Hard mode
        printf("Hard mode: Smoke count not displayed.\n");
    }
}

int artilleryStrike(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk)
{
    char hitShips[4] = {'\0', '\0', '\0', '\0'};
    int index = 0;
    int alreadySunk[256] = {0};

    printf("Performing artillery strike at %d %c!\n", row + 1, col + 'A');

    for (int i = row; i < row + 2 && i < GRID_SIZE; i++)
    {
        for (int j = col; j < col + 2 && j < GRID_SIZE; j++)
        {
            if (board[i][j] == '~')
            {
                printf("Miss at %d %c.\n", i + 1, j + 'A');
                board[i][j] = 'o';
            }
            else if (board[i][j] != '*' && board[i][j] != 'o')
            {
                printf("Hit at %d %c!\n", i + 1, j + 'A');
                hitShips[index++] = board[i][j];
                board[i][j] = '*';
            }
        }
    }

    for (int i = 0; i < index; i++)
    {
        if (!alreadySunk[(unsigned char)hitShips[i]])
        {
            const char *sunkShip = checkShipSunk(board, hitShips[i]);
            if (sunkShip != NULL)
            {
                printf("You sunk the %s!\n", sunkShip);
                alreadySunk[(unsigned char)hitShips[i]] = 1;
                (*shipsSunk)++;
            }
        }
    }

    return 1;
}

void torpedoAttack(char board[GRID_SIZE][GRID_SIZE], char target)
{
    int index;
    bool hit = false;

    if (target >= 'A' && target <= 'J')
    { // Column attack
        index = target - 'A';
        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (board[i][index] != '~' && board[i][index] != '*' && board[i][index] != 'o')
            {
                printf("Hit at row %d, column %c\n", i + 1, target);
                board[i][index] = '*'; // Mark the hit
                hit = true;
            }
        }
    }
    else if (target >= '1' && target <= '9')
    { // Row attack
        index = target - '1';
        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (board[index][i] != '~' && board[index][i] != '*' && board[index][i] != 'o')
            {
                printf("Hit at row %d, column %d\n", index + 1, i + 1);
                board[index][i] = '*';
                hit = true;
            }
        }
    }
    else if (target == '0')
    { // Row attack for row 10
        index = 9;
        for (int i = 0; i < GRID_SIZE; i++)
        {
            if (board[index][i] != '~' && board[index][i] != '*' && board[index][i] != 'o')
            {
                printf("Hit at row %d, column %d\n", index + 1, i + 1);
                board[index][i] = '*';
                hit = true;
            }
        }
    }
    if (!hit)
    {
        printf("Miss\n");
    }
}

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

// Function to check if all ships on the board have been sunk
int checkWin(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            // Check if any ships remain on the board
            if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != 'o')
            {
                return 0; // Game is not over
            }
        }
    }
    return 1; // All ships have been sunk
}
