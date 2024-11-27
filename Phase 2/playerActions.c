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
    if (strcmp(playerName, "megaBot") != 0)
    {
        displayBoard(opponentBoard, showMisses, 1);
    }

    // Display available moves for Player 1
    if (strcmp(playerName, "megaBot") != 0)
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
    printf("  A B C D E F G H I J\n");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%2d ", i + 1); // Row labels
        for (int j = 0; j < GRID_SIZE; j++)
        {
            // Hide ships from the opponent if hideShips is 1
            if (hideShips && (board[i][j] != '*' && board[i][j] != 'o'))
            {
                printf("~ ");
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
