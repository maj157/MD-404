#define GRID_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

int player1SmokeEffectGrid[GRID_SIZE][GRID_SIZE];
int megaBotSmokeEffectGrid[GRID_SIZE][GRID_SIZE];

char player1Radar[GRID_SIZE][GRID_SIZE];
char megaBotRadar[GRID_SIZE][GRID_SIZE];

int megaBotShots[GRID_SIZE][GRID_SIZE];
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void initializeGame(char player1Board[GRID_SIZE][GRID_SIZE], char botBoard[GRID_SIZE][GRID_SIZE],
                    char player1Radar[GRID_SIZE][GRID_SIZE], char megaBotRadar[GRID_SIZE][GRID_SIZE]);
void initializeBoard(char board[GRID_SIZE][GRID_SIZE]);
void initializeRadar(char radar[GRID_SIZE][GRID_SIZE]);
void initializeSmokeEffectGrid();
int askDifficulty();
void clearScreen();
void clearInputBuffer();

void placeFleet(char board[GRID_SIZE][GRID_SIZE], const char *playerName);
int placeShip(char board[GRID_SIZE][GRID_SIZE], int shipSize, const char *shipName, const char *playerName, char shipInitial);

void megaBotPlaceShips(char board[GRID_SIZE][GRID_SIZE]);
void megaBotMakeMove(char opponentBoard[GRID_SIZE][GRID_SIZE], int *shipsSunk, int *gameOver);
int getUnvisitedNeighbors(int row, int col, int neighbors[4][2]);
void markArtilleryImpact(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE]);
int isArtilleryTargetValid(int row, int col, int megaBotShots[GRID_SIZE][GRID_SIZE]);

int executePlayerCommand(int player, char playerName[50], char opponentBoard[GRID_SIZE][GRID_SIZE], char playerRadar[GRID_SIZE][GRID_SIZE],
                         int *shipsSunk, int *smokeUsed, int *radarCount, int showMisses, int *gameOver);
void displayTurnInfo(const char *playerName, char opponentBoard[GRID_SIZE][GRID_SIZE], int showMisses, int shipsSunk);
int getCommandCode(const char *command);
void displayBoard(char board[GRID_SIZE][GRID_SIZE], int showMisses, int hideShips);

int fireAt(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int showMisses);
int radarSweep(char board[GRID_SIZE][GRID_SIZE], char radar[GRID_SIZE][GRID_SIZE], int row, int col, int player);
void applySmokeScreen(char radar[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk, int *smokeUsed, int *lostTurn, int player);
void displaySmokeCount(int showMisses, int currentPlayer, int player1ShipsSunk, int botShipsSunk, int player1SmokeUsed, int player2SmokeUsed);
int artilleryStrike(char board[GRID_SIZE][GRID_SIZE], int row, int col, int *shipsSunk);
void torpedoAttack(char board[GRID_SIZE][GRID_SIZE], char target);
const char *checkShipSunk(char board[GRID_SIZE][GRID_SIZE], char shipChar);

int checkWin(char board[GRID_SIZE][GRID_SIZE]);
