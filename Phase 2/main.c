int main()
{
    char player1Board[GRID_SIZE][GRID_SIZE];
    char botBoard[GRID_SIZE][GRID_SIZE];
    char player1[50] = "Player 1";
    int player1RadarCount = 0;
    int botShipsSunk = 0, player1ShipsSunk = 0;
    int player1SmokeUsed = 0;

    srand(time(NULL));

    initializeGame(player1Board, botBoard, player1Radar, megaBotRadar);
    initializeSmokeEffectGrid();

    int showMisses = askDifficulty();

    // Player 1 places ships
    placeFleet(player1Board, player1);

    // megaBot places ships
    printf("MegaBot is placing its fleet...\n");
    megaBotPlaceShips(botBoard);

    int firstPlayer = rand() % 2;

    int gameOver = 0;
    int currentPlayer = firstPlayer;

    // Main game loop
    while (!gameOver)
    {
        clearScreen();

        printf("Player 1 (%s) - Ships sunk: %d\n", player1, player1ShipsSunk);
        printf("megaBot - Ships sunk: %d\n\n", botShipsSunk);

        displaySmokeCount(showMisses, currentPlayer, player1ShipsSunk, botShipsSunk, player1SmokeUsed, 0);

        if (currentPlayer == 0)
        {

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

            printf("megaBot's turn to play...\n");
            megaBotMakeMove(player1Board, &botShipsSunk, &gameOver);

            if (gameOver)
                break;
            sleep(2);
            currentPlayer = 0;
        }
    }

    return 0;
}
