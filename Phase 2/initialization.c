void initializeGame(char player1Board[GRID_SIZE][GRID_SIZE], char botBoard[GRID_SIZE][GRID_SIZE],
                    char player1Radar[GRID_SIZE][GRID_SIZE], char megaBotRadar[GRID_SIZE][GRID_SIZE])
{

    initializeBoard(player1Board);
    initializeBoard(botBoard);

    initializeRadar(player1Radar);
    initializeRadar(megaBotRadar);

    // Initialize megaBot's shot tracking grid
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            megaBotShots[i][j] = 0; // Unvisited
        }
    }
}

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
            player1SmokeEffectGrid[i][j] = 0; // Clear all smoke effects initially for Player
            megaBotSmokeEffectGrid[i][j] = 0;
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
    printf("\033[H\033[J");
    fflush(stdout);
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
    sleep(2);
    clearScreen();
}

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
