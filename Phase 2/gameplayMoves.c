int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int showMisses)
{
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
            (*shipsSunk)++;

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
    int foundShip = 0;     
    int smokeDetected = 0; 

    // Select the opponent's smoke effect grid based on the current player
    int(*smokeEffectGrid)[GRID_SIZE] = (player == 1) ? megaBotSmokeEffectGrid : player1SmokeEffectGrid;

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
        return 0; 
    }

    if (foundShip)
    {
        printf("Enemy ships found!\n");
    }
    else
    {
        printf("No enemy ships found.\n");
    }

    return foundShip; 
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
    int(*smokeEffectGrid)[GRID_SIZE] = (player == 1) ? player1SmokeEffectGrid : megaBotSmokeEffectGrid;

    if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE)
    {
        for (int i = row; i <= row + 1 && i < GRID_SIZE; i++)
        {
            for (int j = col; j <= col + 1 && j < GRID_SIZE; j++)
            {
                smokeEffectGrid[i][j] = 1; 
            }
        }
        (*smokeUsed)++; 
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
    { 
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
