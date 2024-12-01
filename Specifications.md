// Requires: player1Board, botBoard, player1Radar, and megaBotRadar are valid 2D arrays of size GRID_SIZE x GRID_SIZE.
//           GRID_SIZE must be defined and greater than 0.
//           megaBotShots is a globally declared 2D array of size GRID_SIZE x GRID_SIZE.
// Effects:  Initializes player1Board and botBoard using initializeBoard().
//           Initializes player1Radar and megaBotRadar using initializeRadar().
//           Sets all elements in megaBotShots to 0, marking them as unvisited.
void initializeGame(char player1Board[GRID_SIZE][GRID_SIZE], char botBoard[GRID_SIZE][GRID_SIZE],
                    char player1Radar[GRID_SIZE][GRID_SIZE], char megaBotRadar[GRID_SIZE][GRID_SIZE]) {
    initializeBoard(player1Board);
    initializeBoard(botBoard);

    initializeRadar(player1Radar);
    initializeRadar(megaBotRadar);

    // Initialize megaBot's shot tracking grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            megaBotShots[i][j] = 0; // Unvisited
        }
    }
}

// Requires: GRID_SIZE is a positive integer.
//           board is a 2D array of size GRID_SIZE x GRID_SIZE.
// Effects: Initializes the board by filling every cell with the character '~'.
void initializeBoard(char board[GRID_SIZE][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            board[i][j] = '~';
        }
    }
}

// Requires: radar is a valid 2D array of size GRID_SIZE x GRID_SIZE.
//           GRID_SIZE is defined and greater than 0.
// Effects:  Sets every element in the radar array to '~', representing an unvisited state.
void initializeRadar(char radar[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            radar[i][j] = '~';
        }
    }
}

// Requires: GRID_SIZE is a positive integer.
//           player1SmokeEffectGrid and megaBotSmokeEffectGrid are 
//           2D arrays of size GRID_SIZE x GRID_SIZE.
// Effects: Initializes the smoke effect grids by setting all elements 
//          in both player1SmokeEffectGrid and megaBotSmokeEffectGrid to 0.
void initializeSmokeEffectGrid()
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            player1SmokeEffectGrid[i][j] = 0; // Clear all smoke effects initially for Player
            megaBotSmokeEffectGrid[i][j] = 0;
        }
    }
}

// Requires: No input parameters. The function assumes standard input is available for reading user input.
//           The function uses a helper function `clearInputBuffer()` to clear the input buffer.
// Effects:  Prompts the user to choose a difficulty level for the game.
//           Reads an integer input from the user. 
//           If the input is invalid or not 1 or 2, defaults to hard mode (difficulty = 2).
//           Returns 1 for easy mode (track hits and misses) or 0 for hard mode (track only hits).
int askDifficulty() {
    printf("Welcome to Battleship!\n\n");
    int difficulty;
    printf("Choose tracking difficulty level:\n");
    printf("1. Easy (Track hits and misses)\n");
    printf("2. Hard (Track only hits)\n");
    printf("Enter 1 or 2: ");
    if (scanf("%d", &difficulty) != 1 || (difficulty != 1 && difficulty != 2)) {
        printf("Invalid input. Defaulting to hard mode.\n");
        difficulty = 2;
    }
    clearInputBuffer();
    return (difficulty == 1) ? 1 : 0; // Return 1 for easy, 0 for hard
}

// Requires: The program is running on a supported platform 
//           (_WIN32 for Windows or macOS/Linux).
// Effects: Clears the terminal screen by using the appropriate system command 
//          for the platform and moves the cursor to the top-left corner.
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // Clear screen on Windows
#else
    system("clear"); // Clear screen on macOS/Linux
#endif
    printf("\033[H\033[J");
    fflush(stdout);
}

// Requires: Input buffer contains data.
// Effects: Clears the input buffer by reading and discarding all characters 
//          until a newline ('\n') or EOF is encountered.
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}


