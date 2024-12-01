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

// Requires: board is a valid 2D array of size GRID_SIZE x GRID_SIZE.
//           GRID_SIZE is defined and greater than 0.
//           playerName is a valid, null-terminated string.
//           The function depends on helper functions `placeShip`, `sleep`, and `clearScreen`.
// Effects:  Prompts the specified player to place their fleet on the board.
//           Calls `placeShip` for each ship type (Carrier, Battleship, Destroyer, Submarine),
//           specifying the size, name, and character representation of each ship.
//           Pauses execution for 2 seconds using `sleep` and then clears the screen using `clearScreen`.
void placeFleet(char board[GRID_SIZE][GRID_SIZE], const char *playerName) {
    printf("\n%s, it's time to place your fleet!\n", playerName);
    placeShip(board, 5, "Carrier", playerName, 'C');
    placeShip(board, 4, "Battleship", playerName, 'B');
    placeShip(board, 3, "Destroyer", playerName, 'D');
    placeShip(board, 2, "Submarine", playerName, 'S');
    sleep(2);
    clearScreen();
}

// Requires: 
// - board is a 2D array of size GRID_SIZE x GRID_SIZE.
// - shipSize is a positive integer representing the size of the ship.
// - shipName is a valid string representing the name of the ship.
// - playerName is a valid string representing the player's name.
// - shipInitial is a valid character representing the initial of the ship.
// Effects: 
// - Prompts the player to enter the starting position (row and column) and orientation (horizontal or vertical)
//   for placing the ship.
// - Validates the input to ensure the ship fits within the grid and does not overlap with other ships.
// - Places the ship on the board using the given shipInitial if the placement is valid.
// - Returns 1 if the ship is successfully placed, otherwise prompts the user to re-enter the details.
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char *shipName, const char *playerName, char shipInitial)
{
    int row;
    char col;
    char orientation[20];

    while (1)
    {

        printf("%s, placing your %s (size %d):\n", playerName, shipName, shipSize);
        printf("%s, enter starting position (row column, e.g., 3 B): ", playerName);
        if (scanf("%d %c", &row, &col) != 2)
        {
            printf("Invalid input. Try again.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        col = toupper(col) - 'A';
        row--;

        int validPlacement = 1;

        if (row < 0 || row >= GRID_SIZE || col < 0 || col >= GRID_SIZE)
        {
            printf("Error: Invalid starting position for %s. Row or column out of bounds.\n", shipName);
            continue;
        }

        printf("Enter orientation (horizontal or vertical): ");
        scanf("%19s", orientation); // Limit input to avoid buffer overflow
        clearInputBuffer();

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
            displayBoard(board, 1, 0);
            break;
        }
    }
    return 1;
}




