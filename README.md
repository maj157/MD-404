# Battleship Project Overview

## Introduction
This repository contains our implementation of the Battleship project for **CMPS 270 – Software Construction (Fall 2024)**. The project is divided into two phases:

- **Phase 1**: A two-player version of the Battleship game.
- **Phase 2**: A player-versus-bot version with our custom MegaBot.

The project adheres to the guidelines and grading policy provided, including specifications, testing strategies, and detailed reports.

---

## Phase 1: Two-Player Battleship Game

### Features
- **Game Setup**:
  - Players interact through a console-based interface.
  - A 10x10 grid is initialized for each player with water (`~`).
  - Players place their fleet:
    - **Carrier (5 cells)**
    - **Battleship (4 cells)**
    - **Destroyer (3 cells)**
    - **Submarine (2 cells)**
  - Ships cannot overlap or extend beyond the grid.

- **Gameplay**:
  - Players alternate turns.
  - Available moves:
    - **Fire**: Target a specific cell.
    - **Radar Sweep**: Check for ships in a 2x2 area (limited to 3 uses per game).
    - **Smoke Screen**: Obscure a 2x2 area from radar sweeps (1 allowed per sunk ship).
    - **Artillery**: Target a 2x2 area (unlocked after sinking one ship).
    - **Torpedo**: Target an entire row or column (unlocked after sinking three ships).
  - **Tracking Modes**:
    - **Easy Mode**:
      - Hits and misses are displayed.
      - Players are warned when firing at previously targeted coordinates.
      - Smoke counts are tracked.
    - **Hard Mode**:
      - Only hits are displayed.
      - Players are **not warned** when firing at previously targeted coordinates (we added this idea).
      - Smoke counts are **not tracked** (we added this idea).

- **Endgame**:
  - The game ends when one player sinks all opponent ships.
  - A winner message is displayed.

---

## Phase 2: Player vs. MegaBot

### Enhancements
- Replaced the second player with a bot (MegaBot).
- **Difficulty Modes**:
  - Easy mode: Same rules as phase 1 (easy tracking).
  - Hard mode: Same rules as phase 1 (hard tracking).

### Fixed Issues
- Corrected the implementation of **Artillery** and **Torpedo**:
  - Each move is now **unlocked only once** upon meeting the required conditions ( unlocked only once during the next turn of the player who sinks the other player’s ship in the current turn.) and is limited to one use.
    - **Artillery**: Unlocked after sinking one ship, targeting a 2x2 area.
    - **Torpedo**: Unlocked after sinking three ships, targeting an entire row or column.

---

## MegaBot Strategy
The **MegaBot** strategy is designed to perform better than random and adapt dynamically to the game state. A detailed explanation of its strategy is available in the project report. Key features include:
- **Hunting Mode**: Uses a checkerboard pattern to locate ships.
- **Focused Attack Mode**: Targets neighboring cells after a hit to sink ships efficiently.
- **Queue-Driven Targetting**: Systematically target cells around detected hits (resulted from artillery move 2x2 block) to ensure efficient ship destruction.
- **Artillery and Torpedo Usage**: Strategically deployed when unlocked to maximize impact.

---

## Additional Files

### `Specifications.md`
Contains detailed specifications for all functions used in Phase 2. This file serves as a reference for understanding how each function works.

### `Testing.md`
Includes test cases for specific functions that played a critical role in the game. These cases demonstrate the correctness and robustness of our implementation.

---

## Repository Structure
- `phase1.c`: Code for the two-player Battleship game.
- `phase2.c`: Code for the player-vs-bot Battleship game (MegaBot).
- `Specifications.md`: Function specifications for Phase 2.
- `Testing.md`: Testing strategies and cases.

---

## How to Run
1. Compile the code using `gcc`:
   - Phase 1: `gcc -o phase1 phase1.c`
   - Phase 2: `gcc -o phase2 phase2.c`
2. Run the executable:
   - Phase 1: `./phase1`
   - Phase 2: `./phase2`

### Preferred Development Environment
You can use **Visual Studio Code (VS Code)** as a convenient option for compiling and running the code. VS Code offers an integrated terminal, debugging tools, and extensions like the C/C++ extension by Microsoft, which can help simplify the development process. However, any editor or IDE that supports C programming will work.


## Note for TAs
This README serves as a guide for understanding our Battleship project. For detailed explanations of the MegaBot strategy, refer to the project report. For function-level details, consult `Specifications.md`. For test cases and results, see `Testing.md`.


