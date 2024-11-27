void megaBotPlaceShips(char board[GRID_SIZE][GRID_SIZE])
{
    const int shipSizes[] = {5, 4, 3, 2};
    const char shipChars[] = {'C', 'B', 'D', 'S'};

    for (int i = 0; i < 4; i++)
    {
        int placed = 0;
        while (!placed)
        {
            int prioritizeCenter = rand() % 100 < 70; // 70% chance to prioritize central placement
            int row = prioritizeCenter ? (rand() % (GRID_SIZE / 2)) + GRID_SIZE / 4 : rand() % GRID_SIZE;
            int col = prioritizeCenter ? (rand() % (GRID_SIZE / 2)) + GRID_SIZE / 4 : rand() % GRID_SIZE;
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
        if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE && megaBotShots[newRow][newCol] == 0)
        {
            neighbors[count][0] = newRow;
            neighbors[count][1] = newCol;
            count++;
        }
    }
    return count;
}

void markArtilleryImpact(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE])
{
    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE)
            {
                megaBotShots[i][j] = 1; // Mark cell as visited
            }
        }
    }
}

int isArtilleryTargetValid(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE])
{
    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE)
            {
                if (megaBotShots[i][j] == 1) // Any cell in the block is visited
                    return 0;                // Invalid target
            }
        }
    }
    return 1; // All cells in the block are unvisited
}

void megaBotMakeMove(char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver)
{
    static int lastHitRow = -1, lastHitCol = -1; // Track the last successful hit
    static int artilleryUnlocked = 0;            // Flag to track artillery availability
    static int torpedoUnlocked = 0;              // Flag to track torpedo availability
    int neighbors[4][2];
    int foundTarget = 0;

    if (*shipsSunk >= 3)
        torpedoUnlocked = 1; // Unlock torpedo after sinking 3 ships
    else if (*shipsSunk >= 1)
        artilleryUnlocked = 1; // Unlock artillery after sinking 1 ship

    // Priority 1: Use Torpedo if Unlocked
    if (torpedoUnlocked)
    {
        if (rand() % 2 == 0)
        { // Row attack
            int targetRow = rand() % GRID_SIZE;
            printf("megaBot uses torpedo on row %d\n", targetRow + 1);
            torpedoAttack(opponentBoard, '1' + targetRow);
        }
        else
        { // Column attack
            char targetCol = 'A' + (rand() % GRID_SIZE);
            printf("megaBot uses torpedo on column %c\n", targetCol);
            torpedoAttack(opponentBoard, targetCol);
        }

        // Check if the game is won
        if (checkWin(opponentBoard))
        {
            printf("megaBot wins!\n");
            *gameOver = 1;
        }
        return;
    }

    // Priority 2: Use Artillery if Unlocked
    if (artilleryUnlocked)
    {
        for (int row = 0; row < GRID_SIZE - 1; row++)
        {
            for (int col = 0; col < GRID_SIZE - 1; col++)
            {
                if (isArtilleryTargetValid(row, col, megaBotShots)) // Check if 2x2 block is valid
                {
                    printf("megaBot uses artillery at %d%c\n", row + 1, 'A' + col);
                    artilleryStrike(opponentBoard, row, col, shipsSunk);
                    markArtilleryImpact(row, col, megaBotShots); // Mark the 2x2 block as visited

                    // Check if the game is won
                    if (checkWin(opponentBoard))
                    {
                        printf("megaBot wins!\n");
                        *gameOver = 1;
                    }
                    return;
                }
            }
        }
    }

    // Priority 3: Normal Fire
    if (lastHitRow != -1 && lastHitCol != -1)
    {
        int neighborCount = getUnvisitedNeighbors(lastHitRow, lastHitCol, neighbors);
        if (neighborCount > 0)
        {
            int targetIndex = rand() % neighborCount;
            int row = neighbors[targetIndex][0];
            int col = neighbors[targetIndex][1];

            printf("megaBot fires at %d%c\n", row + 1, 'A' + col);
            if (opponentBoard[row][col] != '~')
            {
                printf("Hit!\n");
                char shipChar = opponentBoard[row][col];
                opponentBoard[row][col] = '*';
                lastHitRow = row;
                lastHitCol = col;

                const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                if (sunkShip != NULL)
                {
                    printf("megaBot sunk the %s!\n", sunkShip);
                    (*shipsSunk)++;
                }
            }
            else
            {
                printf("Miss!\n");
                opponentBoard[row][col] = 'o';
                lastHitRow = -1;
                lastHitCol = -1;
            }

            if (checkWin(opponentBoard))
            {
                printf("megaBot wins!\n");
                *gameOver = 1;
            }
            megaBotShots[row][col] = 1;
            return;
        }
    }

    for (int i = 0; i < GRID_SIZE && !foundTarget; i++)
    {
        for (int j = (i % 2); j < GRID_SIZE; j += 2)
        {
            if (megaBotShots[i][j] == 0)
            {
                printf("megaBot fires at %d%c\n", i + 1, 'A' + j);
                if (opponentBoard[i][j] != '~')
                {
                    printf("Hit!\n");
                    char shipChar = opponentBoard[i][j];
                    opponentBoard[i][j] = '*';
                    lastHitRow = i;
                    lastHitCol = j;

                    const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                    if (sunkShip != NULL)
                    {
                        printf("megaBot sunk the %s!\n", sunkShip);
                        (*shipsSunk)++;
                    }
                }
                else
                {
                    printf("Miss!\n");
                    opponentBoard[i][j] = 'o';
                }

                if (checkWin(opponentBoard))
                {
                    printf("megaBot wins!\n");
                    *gameOver = 1;
                }
                megaBotShots[i][j] = 1;
                return;
            }
        }
    }

    int row, col;
    do
    {
        row = rand() % GRID_SIZE;
        col = rand() % GRID_SIZE;
    } while (megaBotShots[row][col] != 0);

    printf("megaBot fires at %d%c\n", row + 1, 'A' + col);
    if (opponentBoard[row][col] != '~')
    {
        printf("Hit!\n");
        char shipChar = opponentBoard[row][col];
        opponentBoard[row][col] = '*';
        lastHitRow = row;
        lastHitCol = col;

        const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
        if (sunkShip != NULL)
        {
            printf("megaBot sunk the %s!\n", sunkShip);
            (*shipsSunk)++;
        }
    }
    else
    {
        printf("Miss!\n");
        opponentBoard[row][col] = 'o';
    }

    if (checkWin(opponentBoard))
    {
        printf("megaBot wins!\n");
        *gameOver = 1;
    }
    megaBotShots[row][col] = 1;
}
