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

