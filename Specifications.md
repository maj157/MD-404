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

// Requires: board is a valid 2D array of size GRID_SIZE x GRID_SIZE.
//           GRID_SIZE is defined and greater than 0.
//           Each cell of the board is initialized to '~', representing an empty state.
// Effects:  Places four ships (Carrier, Battleship, Destroyer, Submarine) on the board for MegaBot.
//           Ship sizes are 5, 4, 3, and 2 cells, respectively, and are represented by 'C', 'B', 'D', and 'S'.
//           Ships are placed randomly, with a 70% chance to prioritize central positions.
//           Ensures no overlaps between ships and that all ships fit within the board boundaries.
void megaBotPlaceShips(char board[GRID_SIZE][GRID_SIZE]) {
    const int shipSizes[] = {5, 4, 3, 2};
    const char shipChars[] = {'C', 'B', 'D', 'S'};

    for (int i = 0; i < 4; i++) {
        int placed = 0;
        while (!placed) {
            int prioritizeCenter = rand() % 100 < 70; // 70% chance to prioritize central placement
            int row = prioritizeCenter ? (rand() % (GRID_SIZE / 2)) + GRID_SIZE / 4 : rand() % GRID_SIZE;
            int col = prioritizeCenter ? (rand() % (GRID_SIZE / 2)) + GRID_SIZE / 4 : rand() % GRID_SIZE;
            int horizontal = rand() % 2; // 0 = vertical, 1 = horizontal
            placed = 1;

            // Check placement boundaries and overlaps
            if (horizontal) {
                if (col + shipSizes[i] > GRID_SIZE) {
                    placed = 0;
                } else {
                    for (int j = 0; j < shipSizes[i]; j++) {
                        if (board[row][col + j] != '~') {
                            placed = 0;
                            break;
                        }
                    }
                    if (placed) {
                        for (int j = 0; j < shipSizes[i]; j++) {
                            board[row][col + j] = shipChars[i];
                        }
                    }
                }
            } else {
                if (row + shipSizes[i] > GRID_SIZE) {
                    placed = 0;
                } else {
                    for (int j = 0; j < shipSizes[i]; j++) {
                        if (board[row + j][col] != '~') {
                            placed = 0;
                            break;
                        }
                    }
                    if (placed) {
                        for (int j = 0; j < shipSizes[i]; j++) {
                            board[row + j][col] = shipChars[i];
                        }
                    }
                }
            }
        }
    }
}

// Requires: 
// - row and col are valid indices within the bounds of the grid (0 ≤ row, col < GRID_SIZE).
// - neighbors is a 2D array of size 4x2 to store the coordinates of unvisited neighbors.
// - megaBotShots is a 2D array representing the grid state, where 0 indicates unvisited cells.
// - directions is a predefined array of size 4x2 that represents the four possible moves (up, down, left, right).
// Effects: 
// - Checks the four possible neighboring cells (up, down, left, right) of the given position (row, col).
// - If a neighboring cell is within grid bounds and unvisited (megaBotShots[newRow][newCol] == 0),
//   its coordinates are added to the `neighbors` array.
// - Returns the number of valid unvisited neighbors found.
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

// Requires: row and col are valid indices within the range [0, GRID_SIZE - 1].
//           megaBotShots is a valid 2D array of size GRID_SIZE x GRID_SIZE, representing the bot's shot tracking grid.
//           GRID_SIZE is defined and greater than 0.
// Effects:  Marks a 2x2 area of the grid, starting from (row, col), in the megaBotShots array.
//           Sets the value of each valid cell within the 2x2 area to 1, indicating the area impacted by artillery.
//           Ensures no out-of-bounds access by checking that the indices are within the grid boundaries.
void markArtilleryImpact(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE]) {
    for (int i = row; i <= row + 1; i++) {
        for (int j = col; j <= col + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
                megaBotShots[i][j] = 1;
            }
        }
    }
}

// Requires: 
// - row and col are valid indices within the bounds of the grid (0 ≤ row, col < GRID_SIZE).
// - megaBotShots is a 2D array of size GRID_SIZE x GRID_SIZE representing the grid state, 
//   where 1 indicates a visited cell and 0 indicates an unvisited cell.
// Effects: 
// - Checks a 2x2 block of cells starting at the specified (row, col) position.
// - Returns 0 if any cell in the block is visited (megaBotShots[i][j] == 1), and 1 if all cells in the block are unvisited.
// - Ensures all cells in the 2x2 block are within grid boundaries before validation.
int isArtilleryTargetValid(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE])
{
    for (int i = row; i <= row + 1; i++)
    {
        for (int j = col; j <= col + 1; j++)
        {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE)
            {
                if (megaBotShots[i][j] == 1) // Any cell in the block is visited
                    return 0;
            }
        }
    }
    return 1;
}

// Requires: row and col are valid indices within the range [0, GRID_SIZE - 1].
//           opponentBoard is a valid 2D array of size GRID_SIZE x GRID_SIZE, representing the opponent's board.
//           artilleryHits is a valid 2D array of size 4x2, used to store the coordinates of artillery hits.
//           The function depends on a helper function `enqueueNeighbor(int, int)` for queuing valid neighboring cells.
//           GRID_SIZE is defined and greater than 0.
// Effects:  Processes a 2x2 block on the opponent's board starting at (row, col).
//           - Records the coordinates of cells within the block that are hits (not '~' or 'o') in the artilleryHits array.
//           - Calls `enqueueNeighbor` for valid unvisited hits (cells not marked as '*').
//           - Ensures no out-of-bounds access by checking indices against grid boundaries.
//           - Returns the total number of hits found in the 2x2 block.
int processArtilleryHits(int row, int col, char opponentBoard[GRID_SIZE][GRID_SIZE], int artilleryHits[4][2]) {
    int hitCount = 0;
    for (int i = row; i <= row + 1; i++) { // Check 2x2 block
        for (int j = col; j <= col + 1; j++) {
            if (i >= 0 && i < GRID_SIZE && j >= 0 && j < GRID_SIZE) {
                if (opponentBoard[i][j] != '~' && opponentBoard[i][j] != 'o') {
                    artilleryHits[hitCount][0] = i;
                    artilleryHits[hitCount][1] = j;
                    hitCount++;

                    // Only enqueue valid neighbors of unvisited hits
                    if (opponentBoard[i][j] != '*') {
                        enqueueNeighbor(i, j);
                    }
                }
            }
        }
    }

    return hitCount;
}

// Requires:
// - hit is a pointer to a valid Cell structure containing the row and column of the last successful hit.
// - hitCount is the current number of hits made by the megaBot.
// - opponentBoard is a 2D array of size GRID_SIZE x GRID_SIZE representing the opponent's board.
//   - Cells marked with 'o' are missed shots, and cells marked with '*' are already hit.
// - shipsSunk is a pointer to an integer tracking the number of ships sunk so far.
// - gameOver is a pointer to an integer indicating whether the game is over (1 for game over, 0 otherwise).
// Effects:
// - Attempts to fire at all valid neighboring cells (up, down, left, right) around the given `hit`.
// - Calls `fireAt()` to perform the firing action and updates the opponent's board.
// - If a successful hit leads to sinking all ships, calls `checkWin()` to determine if the megaBot has won the game.
// - If `checkWin()` returns true, sets `*gameOver` to 1 and announces the megaBot's victory.
// - Returns early after the first successful fire or if the game is won.
void fireAroundHits(Cell *hit, int hitCount, char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver)
{
    int row = hit->row;
    int col = hit->col;
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

    for (int d = 0; d < 4; d++)
    {
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];

        if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE &&
            opponentBoard[newRow][newCol] != 'o' && opponentBoard[newRow][newCol] != '*')
        {

            int result = fireAt(opponentBoard, newRow, newCol, shipsSunk, 0);

            if (result == 1) // Successful fire
            {
                if (checkWin(opponentBoard))
                {
                    printf("megaBot wins!\n");
                    *gameOver = 1;
                    return;
                }
                return;
            }
        }
    }
}

// Requires: opponentBoard is a valid 2D array of size GRID_SIZE x GRID_SIZE, representing the opponent's board.
//           megaBotShots is a valid 2D array of size GRID_SIZE x GRID_SIZE, tracking MegaBot's visited cells.
//           shipsSunk, gameOver, lastHitRow, lastHitCol, and direction are valid pointers to integers.
//           GRID_SIZE is defined and greater than 0.
//           The function depends on helper functions `checkShipSunk()` and `checkWin()`.
// Effects:  - Simulates MegaBot firing at the opponent's board in a checkerboard pattern.
//           - Fires at the first unvisited cell in the pattern, marking it as visited in megaBotShots.
//           - If a hit occurs:
//               - Updates `lastHitRow` and `lastHitCol` to the coordinates of the hit.
//               - Updates `direction` to suggest a direction for focused sinking if valid unvisited neighbors exist.
//               - Checks if the hit ship is sunk using `checkShipSunk()` and increments `shipsSunk` if a ship is sunk.
//               - Resets `lastHitRow`, `lastHitCol`, and `direction` if a ship is sunk.
//           - If a miss occurs, marks the cell with 'o' on the opponent's board.
//           - Checks for a game-over state using `checkWin()` and sets `gameOver` to 1 if all ships are sunk.
//           - Prints the result of each shot (hit, miss, or sunk ship) and whether the game is over.
//           - Returns 1 if a move was made (either a hit or miss), or 0 if no unvisited cells were available.
int checkerboard(char opponentBoard[GRID_SIZE][GRID_SIZE], int megaBotShots[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver, int *lastHitRow, int *lastHitCol, int *direction) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = (i % 2); j < GRID_SIZE; j += 2) {
            if (megaBotShots[i][j] == 0) { // Check unvisited cells
                printf("megaBot fires at %d%c\n", i + 1, 'A' + j);
                if (opponentBoard[i][j] != '~') { // Hit
                    printf("Hit!\n");
                    char shipChar = opponentBoard[i][j];
                    opponentBoard[i][j] = '*';
                    *lastHitRow = i;
                    *lastHitCol = j;

                    // Determine next direction for focused sinking
                    if (i > 0 && megaBotShots[i - 1][j] == 0)
                        *direction = 0; // Up
                    else if (i < GRID_SIZE - 1 && megaBotShots[i + 1][j] == 0)
                        *direction = 1; // Down
                    else if (j > 0 && megaBotShots[i][j - 1] == 0)
                        *direction = 2; // Left
                    else if (j < GRID_SIZE - 1 && megaBotShots[i][j + 1] == 0)
                        *direction = 3; // Right

                    // Check if a ship was sunk
                    const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                    if (sunkShip != NULL) {
                        printf("megaBot sunk the %s!\n", sunkShip);
                        (*shipsSunk)++;
                        *lastHitRow = -1; // Reset for new ship
                        *lastHitCol = -1;
                        *direction = -1;
                    }
                } else { // Miss
                    printf("Miss!\n");
                    opponentBoard[i][j] = 'o';
                }

                // Check if the game is over
                if (checkWin(opponentBoard)) {
                    printf("megaBot wins!\n");
                    *gameOver = 1;
                    return 1; // Signal that a move was made
                }

                megaBotShots[i][j] = 1; // Mark as visited
                return 1; // Signal that a move was made
            }
        }
    }
    return 0; // No move made
}

// Requires:
// - opponentBoard is a 2D array of size GRID_SIZE x GRID_SIZE representing the opponent's board.
//   - '~' indicates an empty cell, 'o' indicates a missed shot, '*' indicates a hit ship segment, 
//     and other characters represent ship parts.
// - shipsSunk is a pointer to an integer tracking the number of ships sunk by megaBot.
// - gameOver is a pointer to an integer indicating whether the game is over (1 for game over, 0 otherwise).
// Effects:
// - Executes megaBot's turn, prioritizing moves in the following order:
//   1. Fires at unresolved neighboring cells from a queue if any exist.
//   2. Uses torpedo attack if unlocked (after sinking 3 ships) and no unresolved neighbors remain.
//   3. Follows up on consecutive hits for focused sinking of a ship if a previous hit exists.
//   4. Uses artillery strike if unlocked (after sinking 1 ship) and no focused sinking is active.
//   5. Performs checkerboard firing as a default search mechanism if no higher priority action is available.
// - Updates the opponent's board and megaBot's shot tracking grid (megaBotShots).
// - Tracks successful hits and sinks ships by checking neighboring cells and processing focused sinking if needed.
// - Unlocks special attacks (torpedo and artillery) based on the number of ships sunk.
// - Checks for a win after each move and sets *gameOver to 1 if all ships are sunk.
// - Returns early if a win condition is detected or a prioritized action is completed.
void megaBotMakeMove(char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver)
{
    static int lastHitRow = -1, lastHitCol = -1; // Track the last successful hit
    static int direction = -1;                   // Track the current direction: 0 = up, 1 = down, 2 = left, 3 = right
    static int artilleryUnlocked = 0;
    static int torpedoUnlocked = 0;
    static int torpedoedRows[GRID_SIZE] = {0}; // Track rows targeted by torpedo (0 = not targeted, 1 = targeted)

    if (*shipsSunk >= 3)
        torpedoUnlocked = 1; // Unlock torpedo after sinking 3 ships
    else if (*shipsSunk >= 1)
        artilleryUnlocked = 1; // Unlock artillery after sinking 1 ship

    // Priority 1: Process unresolved neighbors from the queue
    if (!isNeighborQueueEmpty())
    {
        Cell nextTarget = dequeueNeighbor();
        int row = nextTarget.row;
        int col = nextTarget.col;

        if (opponentBoard[row][col] == '*' || opponentBoard[row][col] == 'o')
        {
            return;
        }

        // Fire at this neighbor
        printf("megaBot fires at %d%c (from queue)\n", row + 1, 'A' + col);
        if (opponentBoard[row][col] != '~')
        {
            printf("Hit!\n");
            char shipChar = opponentBoard[row][col];
            opponentBoard[row][col] = '*';
            lastHitRow = row;
            lastHitCol = col;

            // Switch to focused sinking for this new ship
            if (direction == -1)
            {
                if (row > 0 && megaBotShots[row - 1][col] == 0)
                    direction = 0;
                else if (row < GRID_SIZE - 1 && megaBotShots[row + 1][col] == 0)
                    direction = 1;
                else if (col > 0 && megaBotShots[row][col - 1] == 0)
                    direction = 2;
                else if (col < GRID_SIZE - 1 && megaBotShots[row][col + 1] == 0)
                    direction = 3;
            }

            const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
            if (sunkShip != NULL)
            {
                printf("megaBot sunk the %s!\n", sunkShip);
                (*shipsSunk)++;
                lastHitRow = -1;
                lastHitCol = -1;
                direction = -1;
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
        return;
    }

    // Priority 2: Use Torpedo if Unlocked and Queue is Empty
    if (torpedoUnlocked)
    {
        for (int row = 0; row < GRID_SIZE; row++)
        {
            if (torpedoedRows[row] == 0)
            {
                printf("megaBot uses torpedo on row %d\n", row + 1);
                torpedoAttack(opponentBoard, '1' + row);
                torpedoedRows[row] = 1;

                if (checkWin(opponentBoard))
                {
                    printf("megaBot wins!\n");
                    *gameOver = 1;
                    return;
                }
                return;
            }
        }
    }

    // Priority 3: Follow Consecutive Hits
    if (lastHitRow != -1 && lastHitCol != -1)
    {
        int newRow = lastHitRow, newCol = lastHitCol;

        // Determine next cell based on direction
        if (direction == 0)
            newRow--;
        else if (direction == 1)
            newRow++;
        else if (direction == 2)
            newCol--;
        else if (direction == 3)
            newCol++;

        if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE && megaBotShots[newRow][newCol] == 0)
        {
            printf("megaBot fires at %d%c (focused sinking)\n", newRow + 1, 'A' + newCol);

            if (opponentBoard[newRow][newCol] != '~')
            {
                printf("Hit!\n");
                char shipChar = opponentBoard[newRow][newCol];
                opponentBoard[newRow][newCol] = '*';
                lastHitRow = newRow;
                lastHitCol = newCol;

                const char *sunkShip = checkShipSunk(opponentBoard, shipChar);
                if (sunkShip != NULL)
                {
                    printf("megaBot sunk the %s!\n", sunkShip);
                    (*shipsSunk)++;
                    lastHitRow = -1;
                    lastHitCol = -1;
                    direction = -1;
                }
            }
            else
            {
                printf("Miss!\n");
                opponentBoard[newRow][newCol] = 'o';
                direction = -1;
            }

            if (checkWin(opponentBoard))
            {
                printf("megaBot wins!\n");
                *gameOver = 1;
            }
            megaBotShots[newRow][newCol] = 1;
            return;
        }
        else
        {
            direction = -1;
        }
    }

    // Priority 4: Use Artillery if Unlocked
    if (artilleryUnlocked)
    {
        for (int row = 0; row < GRID_SIZE - 1; row += 2)
        {
            for (int col = 0; col < GRID_SIZE - 1; col += 2)
            {
                if (isArtilleryTargetValid(row, col, megaBotShots))
                {
                    printf("megaBot uses artillery at %d%c\n", row + 1, 'A' + col);
                    artilleryStrike(opponentBoard, row, col, shipsSunk);
                    markArtilleryImpact(row, col, megaBotShots);

                    // Process hits in the 2x2 area
                    int artilleryHits[4][2];
                    int hitCount = processArtilleryHits(row, col, opponentBoard, artilleryHits);

                    // Enqueue valid neighbors of each hit
                    for (int i = 0; i < hitCount; i++)
                    {
                        int hitRow = artilleryHits[i][0];
                        int hitCol = artilleryHits[i][1];

                        for (int d = 0; d < 4; d++)
                        {
                            int newRow = hitRow + directions[d][0];
                            int newCol = hitCol + directions[d][1];

                            if (newRow >= 0 && newRow < GRID_SIZE &&
                                newCol >= 0 && newCol < GRID_SIZE &&
                                opponentBoard[newRow][newCol] != '*' &&
                                opponentBoard[newRow][newCol] != 'o' &&
                                megaBotShots[newRow][newCol] == 0)
                            {
                                enqueueNeighbor(newRow, newCol);
                            }
                        }
                    }
                    return;
                }
            }
        }
    }
    // Priority 5: Checkerboard Firing (Default Search)
    if (checkerboard(opponentBoard, megaBotShots, shipsSunk, gameOver, &lastHitRow, &lastHitCol, &direction))
        return; 

}

// Requires: 
// - player is a valid player identifier.
// - playerName is a valid, null-terminated string with a maximum length of 50 characters.
// - opponentBoard and playerRadar are valid 2D arrays of size GRID_SIZE x GRID_SIZE.
// - shipsSunk, smokeUsed, radarCount, showMisses, and gameOver are valid pointers to integers.
// - GRID_SIZE is defined and greater than 0.
// - The function depends on helper functions:
//   - `clearInputBuffer()` for clearing the input buffer.
//   - `getCommandCode()` for translating user commands into codes.
//   - `fireAt()`, `torpedoAttack()`, `radarSweep()`, `artilleryStrike()`, and `applySmokeScreen()` for handling moves.
//   - `checkWin()` and `checkShipSunk()` for game state checks.
// Effects: 
// - Prompts the player to enter a command ('fire', 'radar', 'artillery', 'torpedo', or 'smoke').
// - Executes the corresponding action based on the command entered:
//   - **Fire**: Attempts to fire at specified coordinates on the opponent's board. Updates `shipsSunk`, checks for game-over state, and prints results.
//   - Torpedo: If unlocked (requires sinking 3 ships), attacks a row or column of the opponent's board.
//   - **Radar**: If available (maximum of 3 uses), performs a radar sweep and updates `radarCount`.
//   - **Artillery**: If unlocked (requires sinking 1 ship), performs an artillery strike on a 2x2 block on the opponent's board.
//   - **Smoke**: If unlocked (requires sinking at least 1 ship), applies a smoke screen at specified coordinates on the player’s radar.
// - Handles invalid inputs by printing error messages and may forfeit the player's turn.
// - Checks for game-over state after each valid command and sets `*gameOver` to 1 if the game is won.
// - Returns 1 if a valid move is executed, 0 otherwise.
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

Here’s the specification for the displayTurnInfo function:

c
Copy code
// Requires:
// - playerName is a valid string representing the current player’s name.
// - opponentBoard is a 2D array of size GRID_SIZE x GRID_SIZE representing the opponent’s board.
//   - '~' indicates an empty cell, 'o' indicates a missed shot, '*' indicates a hit ship segment, 
//     and other characters represent ship parts.
// - showMisses is a flag (integer) where 1 enables showing missed shots ('o') and 0 hides them.
// - shipsSunk is an integer representing the number of ships sunk by the current player.
// Effects:
// - Displays the turn information for the specified player:
//   - Prints a message indicating it is the player’s turn to play.
//   - If the player is not "megaBot", displays the opponent’s board using `displayBoard()` 
//     with options to show or hide missed shots based on the `showMisses` flag.
//   - If the player is not "megaBot", prints a list of available moves based on the number of ships sunk:
//     - Default moves: "Fire", "Radar"
//     - Additional moves unlocked:
//       - "Torpedo" after sinking 3 ships.
//       - "Artillery" after sinking 1 ship.
//       - "Smoke" after sinking at least 1 ship.
void displayTurnInfo(const char *playerName, char opponentBoard[GRID_SIZE][GRID_SIZE], int showMisses, int shipsSunk)
{
    printf("\n%s's turn to play!\n", playerName);

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

// Requires: 
// - command is a valid, null-terminated string.
// Effects: 
// - Compares the input string `command` against predefined valid commands: "fire", "torpedo", "radar", "artillery", and "smoke".
// - Returns:
//   - 1,2,3,4,5 if `command` matches "fire", "torpedo", "radar", "artillery", "smoke", respectively.
//   - 0 if `command` does not match any valid command.
int getCommandCode(const char *command) {
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

// Requires:
// - board is a 2D array of size GRID_SIZE x GRID_SIZE representing the state of the game board.
//   - '~' represents an empty cell, '*' represents a hit ship segment, 'o' represents a missed shot,
//     and other characters represent parts of ships.
// - showMisses is a flag (integer):
//   - If 1, missed shots ('o') are displayed on the board.
//   - If 0, missed shots are hidden and displayed as '~'.
// - hideShips is a flag (integer):
//   - If 1, ship parts are hidden unless they are hit ('*') or missed ('o').
//   - If 0, all cells, including ship parts, are displayed.
// Effects:
// - Prints the game board to the console with column labels ('A' to 'J') and row labels ('1' to GRID_SIZE).
// - Displays cells based on the following rules:
//   - Ships are hidden unless `hideShips` is 0 or the ship segment is hit ('*').
//   - Missed shots ('o') are shown only if `showMisses` is 1.
//   - Empty cells ('~') are always displayed unless overridden by the conditions above.
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
                printf("~ ");
            }
            else
            {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Requires: 
// - board is a valid 2D array of size GRID_SIZE x GRID_SIZE.
// - row and col are valid indices within the range [0, GRID_SIZE - 1].
// - shipsSunk is a valid pointer to an integer that tracks the number of ships sunk.
// - showMisses is an integer flag indicating whether to display missed shots (1 = show, 0 = hide).
// - GRID_SIZE is defined and greater than 0.
// - The function depends on a helper function `checkShipSunk()` to determine if a ship is sunk.
// Effects: 
// - Fires at the specified (row, col) location on the board.
// - If the location is already marked as hit ('*') or missed ('o') and `showMisses` is 1, returns -1 to indicate invalid action.
// - If the location is '~', it marks the cell as a miss ('o') and prints "Miss!".
// - If the location contains a ship (any other character), marks the cell as hit ('*'), prints "Hit!", and checks if the ship is sunk:
//   - If the ship is sunk, increments `shipsSunk`, prints the name of the sunk ship, and unlocks special moves:
//     - Unlocks "Artillery" after sinking 1 ship.
//     - Unlocks "Torpedo" after sinking 3 ships.
// - Returns 1 if the shot is valid (hit or miss).
int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int showMisses)
{
    if ((board[row][col] == '*' || board[row][col] == 'o') && showMisses == 1)
    {
        return -1;
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

            if (*shipsSunk == 1)
                printf("Artillery unlocked!\n");
            if (*shipsSunk >= 3)
                printf("Torpedo unlocked!\n");
        }
    }

    return 1;
}

// Requires:
// - board is a 2D array of size GRID_SIZE x GRID_SIZE representing the state of the game board.
//   - '~' represents an empty cell, '*' represents a hit ship segment, 'o' represents a missed shot, 
//     and other characters represent parts of ships.
// - radar is a 2D array of size GRID_SIZE x GRID_SIZE representing the radar's scanned state.
// - row and col are valid indices within the bounds of the grid (0 ≤ row, col < GRID_SIZE).
// - player is an integer indicating the current player (1 for Player 1, any other value for Player 2/megaBot).
// Effects:
// - Performs a radar sweep over a 2x2 grid area starting at the specified (row, col).
// - Checks for the presence of smoke effects in the specified 2x2 area:
//   - If smoke is detected, clears the smoke effect in the 2x2 area for the current player and prints "No enemy ships found."
//   - If no smoke is detected, scans for enemy ships (cells that are neither '~', '*', nor 'o') in the 2x2 area.
// - Prints "Enemy ships found!" if at least one ship segment is detected, otherwise prints "No enemy ships found."
// - Returns 1 if an enemy ship is found, 0 otherwise.
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

// Requires:
// - opponentRadar is a valid 2D array of size GRID_SIZE x GRID_SIZE.
// - row and col are valid integers representing coordinates within the range [0, GRID_SIZE - 1].
// - shipsSunk, smokeUsed, and lostTurn are valid pointers to integers.
// - player is a valid player identifier (1 for Player 1, otherwise for MegaBot).
// - GRID_SIZE is defined and greater than 0.
// - The function depends on globally defined 2D arrays: player1SmokeEffectGrid and megaBotSmokeEffectGrid.
// Effects:
// - Checks if the player has used more smoke screens than the number of ships sunk.
//   - If true, prints a message, sets `*lostTurn` to 1, and forfeits the player's turn.
// - If the coordinates are valid:
//   - Applies a smoke screen to a 2x2 block on the `smokeEffectGrid` starting at (row, col).
//   - Marks the affected cells with `1` in the `smokeEffectGrid`.
//   - Increments `*smokeUsed` and prints the smoke screen coordinates.
// - If the coordinates are invalid, prints an error message and does not apply the smoke screen.
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

// Requires:
// - showMisses is an integer flag:
//   - If 1, smoke count is displayed.
//   - If 0, smoke count is hidden (hard mode).
// - currentPlayer is an integer representing the current player (0 for Player 1, 1 for Player 2/megaBot).
// - player1ShipsSunk and player2ShipsSunk are integers representing the number of ships sunk by Player 1 and Player 2, respectively.
// - player1SmokeUsed and player2SmokeUsed are integers representing the number of smoke effects used by Player 1 and Player 2, respectively.
// Effects:
// - If `showMisses` is 1:
//   - Calculates the remaining smoke effects for the current player based on the difference between ships sunk and smoke effects used.
//   - Prints the remaining smoke effects for the current player in the format: "Player X, you have Y smoke(s) left."
// - If `showMisses` is 0:
//   - Prints "Hard mode: Smoke count not displayed."
void displaySmokeCount(int showMisses, int currentPlayer, int player1ShipsSunk, int player2ShipsSunk, int player1SmokeUsed, int player2SmokeUsed)
{
    if (showMisses == 1)
    { 
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

// Requires:
// - board is a valid 2D array of size GRID_SIZE x GRID_SIZE.
// - row and col are valid integers representing the top-left corner of the 2x2 area to strike, within the range [0, GRID_SIZE - 1].
// - shipsSunk is a valid pointer to an integer tracking the number of ships sunk.
// - GRID_SIZE is defined and greater than 0.
// - The function depends on the helper function `checkShipSunk()` to determine if a ship is sunk.
// Effects:
// - Performs an artillery strike on a 2x2 area of the board starting at (row, col):
//   - If a cell contains '~', it marks the cell as a miss ('o') and prints a "Miss" message.
//   - If a cell contains a ship, it marks the cell as hit ('*'), prints a "Hit" message, and tracks the hit ship in the `hitShips` array.
// - After the strike, checks if any ships were sunk:
//   - For each unique hit ship, calls `checkShipSunk()`.
//   - If a ship is sunk, increments `*shipsSunk` and prints the name of the sunk ship.
// - Returns 1 to indicate the strike was executed.
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










