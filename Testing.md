# Testing Report

## 1. Introduction

This report outlines the testing process for the Battleship project. The primary goal of testing is to ensure the program behaves as expected, handles user interactions correctly, and produces reliable results under various scenarios. 

Key objectives of the testing process:
- Verify that core game functionalities, such as firing at targets, ship placement, sinking ships, bot actions & placements, work as intended.
- Ensure stability by handling edge cases, including invalid inputs, boundary conditions, and repetitive actions.
- Validate the interactions between different components of the game, such as player commands and board updates.
- Confirm that the program remains stable and does not crash under unusual or unexpected scenarios.

The testing process includes trying the program manually, testing specific parts of the code, and checking unusual cases to make sure everything works as expected.

## 2. Testing Strategy

The testing strategy for this project includes the following approaches:

- **Manual Testing**: 
  - Most of the testing was done manually by running the program and entering various inputs to see how it behaves.
  - Different parts of the game were tested separately, such as firing at targets, placing ships, and bot actions.

- **Automated Testing**:
  - We didn’t use automated testing tools in this project as it’s our first time doing a testing report. 

- **Edge Case Handling**:
  - We tested unusual and extreme scenarios, like firing at invalid coordinates, placing ships over boundaries, applying smoke screens at the edges of the board, and trying invalid inputs that weren’t allowed. This helped us ensure the game doesn’t break in unexpected situations.

This strategy helped us check if the game worked correctly and prepared us to handle edge cases better.

## 3. Test Cases for Key Functions

### Testing Cases for Ship Placement

**Context**:  
The `placeShip` function is responsible for allowing players to place their ships on the grid. It ensures ships are placed within the grid, do not overlap with existing ships, and follow valid orientations. This function plays a critical role in setting up the game correctly.

#### **Function: `placeShip`**

**Description**:  
The `placeShip` function takes input for the starting position, orientation (horizontal or vertical), and ship size, then places the ship on the board if all conditions are met. It handles invalid inputs, overlapping ships, and out-of-bound placements gracefully.

**Test Cases**:
| Test Case                        | Input                            | Expected Output                                         | Result  |
|----------------------------------|----------------------------------|---------------------------------------------------------|---------|
| Valid horizontal placement       | `3B`, `horizontal`               | Ship placed at B3, C3, D3                               | Passed  |
| Valid vertical placement         | `2E`, `vertical`                 | Ship placed at E2, E3, E4, E5                           | Passed  |
| Out of bounds starting position  | `11 C`                           | "Error: Invalid starting position."                     | Passed  |
| Invalid orientation              | `orientation = diagonal`         | "Invalid orientation. Try again."                       | Passed  |
| Horizontal exceeds boundaries    | `3H`, `horizontal`               | "Error: Ship placement exceeds grid boundaries."        | Passed  |
| Vertical exceeds boundaries      | `8C`, `vertical`                 | "Error: Ship placement exceeds grid boundaries."        | Passed  |
| Overlapping horizontal ships     |   `3B`                           | "Error: Ship overlaps with another ship."               | Passed  |
| Overlapping vertical ships       | `2D`                             | "Error: Ship overlaps with another ship."               | Passed  |


### **Testing Approach**

1. **Valid Placements**:
   - Test horizontal and vertical placements to ensure ships are placed correctly on the grid.
   - Confirm that valid placements update the board as expected.

2. **Out-of-Bound Placements**:
   - Test placements where the ship exceeds grid boundaries (both horizontally and vertically).
   - Confirm that the function handles these scenarios gracefully by rejecting the input.

3. **Overlapping Ships**:
   - Place one ship, then attempt to place another ship overlapping the first.
   - Ensure the function detects overlaps and prompts the player to try again.

4. **Invalid Input Handling**:
   - Enter invalid row/column combinations or orientations (e.g., diagonal, invalid row number).
   - Verify that the function prompts the user to correct the input.

### Function: `fireAt`

**Description**:
The `fireAt` function is one of the most critical parts of the game. It handles firing at a specific location on the board, updating the board with hits (`*`) or misses (`o`), and checking if any ships were sunk. This function directly impacts the gameplay, so testing it thoroughly was essential.

**Test Cases**:

| Test Case                          | Input                                  | Expected Output                             | Result  |
|------------------------------------|----------------------------------------|---------------------------------------------|---------|
| Normal hit                         | `row = 2, col = C`                     | "Hit!" Board updated to `'*'`               | Passed  |
| Miss                               | `row = 4, col = E`                     | "Miss!" Board updated to `'o'`              | Passed  |
| Already fired location (hit)       | `row = 2, col = C`                     | "Already fired at this location."           | Passed  |
| Invalid coordinates (negative row) | `row = -1, col = B`                    | "Invalid coordinates. You lost your turn!"  | Passed  |
| Invalid coordinates (out of bounds)| `row = 11, col = K`                    | "Invalid coordinates. You lost your turn!"  | Passed  |

### Testing Cases for Smoke and Radar Features (1v1)

**Context**:  
These testing cases were more significant during the 2-player version of the game, our bot doesn't use radar or smoke. The mechanics for smoke and radar needed to ensure:
- A player’s radar could detect ships in non-smoked areas but not in smoked ones.
- Smoke applied by a player would not block their own radar scans.
- Smoke effects lasted only until other player applies radar once in the same grid location, ensuring fair gameplay in the next round.

#### **Function: `radarSweep`**

**Description**:  
The `radarSweep` function checks a 2x2 grid starting at `(row, col)` on the opponent’s board and updates the player’s radar grid with information about detected ships.If any part of the block is outside the grid, the coordinates are considered invalid, and the player loses their turn.

**Test Cases**:

| Test Case                              | Input (Player A/B)                       | Expected Output                             | Result  |
|----------------------------------------|------------------------------------------|---------------------------------------------|---------|
| Player A applied smoke                 | `row = 3, col = C`                       | "Smoke applied on row 3"                    | Passed  |
| Player B applies radar                 | `row = 3, col = C`                       | "No ships detected"                         | Passed  |
| Player B applies radar                 | `row = 4, col = C`                       | Ships detected if present                   | Passed  |
| Invalid coordinates (negative row)     | `row = -1, col = B`                      | "Invalid coordinates. You lost your turn!"  | Passed  |
| Invalid coordinates (out of bounds)    | `row = 11, col = K`                      | "Invalid coordinates. You lost your turn!"  | Passed  |

#### **Function: `applySmokeScreen`**

**Description**:  
The `applySmokeScreen` function applies smoke to a 2x2 grid starting at `(row, col)` on the player's radar. The smoke hides ships from radar scans but lasts for only one round.If any part of the block is outside the grid, the coordinates are considered invalid, and the player loses their turn.

**Test Cases**:

| Test Case                              | Input (Player A/B)                       | Expected Output                             | Result  |
|----------------------------------------|------------------------------------------|---------------------------------------------|---------|
| Player A smokes their own grid         | `row = 3, col = C`                       | Smoke applied.                              | Passed  |
| Player B scans smoked grid (same turn) | `row = 3, col = C`                       | "No ships detected."                        | Passed  |
| Player B scans smoked grid (next turn) | `row = 3, col = C`                       | Ships detected if present                   | Passed  |
| Invalid coordinates (negative row)     | `row = -1, col = B`                      | "Invalid coordinates. You lost your turn!"  | Passed  |
| Invalid coordinates (out of bounds)    | `row = 11, col = K`                      | "Invalid coordinates. You lost your turn!"  | Passed  |

#### **Function: `displaySmokeCount`**

**Description**:  
The `displaySmokeCount` function provides the players with information about how many smoke screens they have used relative to the number of ships they have sunk.

**Test Cases**:

| Test Case                              | Input (Player A/B)                       | Expected Output                             | Result  |
|----------------------------------------|------------------------------------------|---------------------------------------------|---------|
| Player A uses one smoke screen         | `shipsSunk = 1, smokeUsed = 1`           | "You have 0 smoke screens remaining."       | Passed  |
| Player A uses more than allowed smoke  | `shipsSunk = 1, smokeUsed = 2`           | "You cannot use more smoke screens!"        | Passed  |
| Player B uses no smoke screens         | `shipsSunk = 2, smokeUsed = 0`           | "You have 2 smoke screens remaining."       | Passed  |
| Player A uses smoke after sinking ship | `shipsSunk = 2, smokeUsed = 1`           | "You have 1 smoke screen remaining."        | Passed  |


### Assumptions for Smoke and Radar Testing
- **Player-Specific Grids**: Each player has a separate radar and smoke grid. Smoke applied by one player does not affect their own radar scans but does block the opponent’s radar.
- **Smoke Duration**: Smoke lasts for only one round. In the next round, radar scans on the same location will detect ships if present.
- **Valid Coordinates**: All functions handle invalid coordinates gracefully, printing `"Invalid coordinates. You lost your turn!"` and skipping the player's turn.

### Function: `artilleryStrike`

**Description**:
The `artilleryStrike` function performs a 2x2 block attack starting from a given coordinate on the board. It updates the board with hits (`*`) and misses (`o`) and checks if any ships were sunk. 

**Assumption**:
- For an artillery strike to be valid, the **entire 2x2 block** must fit within the board boundaries. If any part of the block is outside the grid, the coordinates are considered invalid, and the player loses their turn.

**Test Cases**:

| Test Case                          | Input                                  | Expected Output                             | Result  |
|------------------------------------|----------------------------------------|--------------------------------------------|---------|
| 2x2 block with hits and misses     | `row = 1, col = A`                     | Hits and misses printed; board updated     | Passed  |
| Entire 2x2 block misses            | `row = 4, col = D`                     | "Miss at ..." for all cells; board updated | Passed  |
| Edge case: bottom-right boundary   | `row = 10, col = J`                    | "Invalid coordinates. You lost your turn!" | Passed  |
| Invalid coordinates (negative row) | `row = -1, col = B`                    | "Invalid coordinates. You lost your turn!" | Passed  |
| Invalid coordinates (out of bounds)| `row = 11, col = K`                    | "Invalid coordinates. You lost your turn!" | Passed  |
| 2x2 block sinks a ship             | `row = 2, col = B` (with `shipChar`)   | "You sunk the [ship name]!" printed        | Passed  |

### Testing Cases for Torpedo Attack, Ship Sinking, and Winning Logic

#### **Function: `torpedoAttack`**

**Description**:  
The `torpedoAttack` function performs an attack along an entire row or column, targeting all cells in that line. The target can be specified as:
- A **column** (A–J): Attacks all rows in the given column (case sensitive).
- A **row** (1–10): Attacks all columns in the given row.

**Test Cases**:

| Test Case                          | Input                                  | Expected Output                             | Result  |
|------------------------------------|----------------------------------------|---------------------------------------------|---------|
| Torpedo column attack (valid)      | `C`                                    | Hits/misses printed for all rows in column  | Passed  |
| Torpedo row attack (valid)         | `3`                                    | Hits/misses printed for all columns in row  | Passed  |
| Torpedo misses entire target       | `A` (no ships in column A)             | "Miss" printed                              | Passed  |
| Edge case: invalid column          | `a`                                    | "Invalid input. You lost your turn!"        | Passed  |
| Edge case: invalid row             | `-1`                                   | "Invalid input. You lost your turn!"        | Passed  |

#### **Function: `checkWin`**

**Description**:  
The `checkWin` function checks if the game has been won by scanning the board to determine if any ship cells remain. If no ship cells remain, the function returns `1` to indicate the game is over. Otherwise, it returns `0`.

**Test Cases**:

| Test Case                          | Input                                  | Expected Output                             | Result  |
|------------------------------------|----------------------------------------|---------------------------------------------|---------|
| All ships sunk                     | Board with only `~`, `*`, and `o`      | Returns `1`                                 | Passed  |
| Ships still remaining              | Board with `C` or other ship chars     | Returns `0`                                 | Passed  |
| Empty board                        | Board with all `~`                     | Returns `1`                                 | Passed  |

**Winning Logic**
   - The game ends when no ship cells remain on the board.

### Testing Cases for MegaBot Strategy

**Context**:  
The MegaBot implements a highly adaptive and efficient strategy in the game of Battleship, transitioning between different modes (Hunting Mode, Focused Attack Mode, Artillery, and Torpedo Moves) based on the game state. The following tests were conducted to ensure that each phase of the strategy works as expected. 

#### **Function: `megaBotMakeMove`**

**Description**:  
The `megaBotMakeMove` function drives MegaBot's decision-making process. It transitions through different modes of attack:
1. **Hunting Mode**: MegaBot fires in a checkerboard pattern to locate ships.
2. **Focused Attack Mode**: After a hit, MegaBot fires at neighboring cells until the ship is sunk.
3. **Artillery Move**: After sinking one ship, MegaBot uses artillery to target 2x2 blocks in a checkerboard manner.
4. **Queue-Driven Targeting**: MegaBot processes neighboring cells from the queue if a hit occurs in a 2x2 block resulted from the artillery move.
5. **Torpedo Move**: After sinking three ships, MegaBot uses the torpedo move to target entire row.

**Test Cases**:

| Test Case                               | Input                     | Expected Output                                                                 | Result  |
|-----------------------------------------|---------------------------|---------------------------------------------------------------------------------|---------|
| Initial hunting mode (checkerboard)     | Empty board               | MegaBot fires at alternating cells in a checkerboard pattern (e.g., A1, A3).    | Passed  |
| Transition to Focused Attack Mode       | `Hit at A1`               | MegaBot fires at A2, A3 (neighboring cells) until the ship is sunk.             | Passed  |
| Post-sinking behavior                   | `Ship sunk at A1`         | MegaBot returns to checkerboard firing                                          | Passed  |
| Artillery strike includes a hit         | `Artillery at B2 (hit)`   | Neighboring cells of B3 are added to the queue, and MegaBot fires from queue.   | Passed  |
| Queue-driven targeting works            | `Queue = {B3, C3}`        | MegaBot processes all neighbors before returning to checkerboard mode.          | Passed  |
| Unlocking torpedo after three ships     | `Three ships sunk`        | MegaBot begins torpedo attacks on rows or columns.                              | Passed  |
| Torpedo attack on row with no hits      | `Torpedo on row 1`        | MegaBot prints "Miss" and moves to the next row.                                | Passed  |
| Invalid input (row out of bounds)       | `Torpedo on row 2 `       | MegaBot prints "Invalid input. You lost your turn!"                             | Passed  |

#### 1. Hunting Mode (Checkerboard Pattern)
- **Test Objective**: Ensure MegaBot fires in a checkerboard pattern when searching for ships.
- **Test Execution**:
  - Verified that MegaBot fires at alternating cells (e.g., A1, A3, A5, etc.).
  - Confirmed no redundant shots in the same location.
- **Expected Behavior**: MegaBot covers the board systematically with minimal overlap.

#### 2. Focused Attack Mode (Consecutive Hits)
- **Test Objective**: Verify MegaBot transitions to focused attacks after a hit and sinks the ship efficiently.
- **Test Execution**:
  - Placed a single ship at A1–A3.
  - Confirmed MegaBot continues firing at consecutive cells until the ship is sunk.
- **Expected Behavior**:
  - MegaBot targets A2, then A3, and announces the ship is sunk.

#### 3. Artillery Mode (2x2 Attack)
- **Test Objective**: Ensure MegaBot uses artillery once unlocked to target 2x2 blocks after sinking a ship.
- **Test Execution**:
  - Checked that hits within the 2x2 block enqueue neighboring cells for further targeting.
- **Expected Behavior**:
  - MegaBot adds all valid neighbors of a hit to the queue and systematically processes them.

#### 4. Torpedo Mode (Row/Column Attack)
- **Test Objective**: Test MegaBot’s use of torpedo attack once after three ships are sunk.
- **Test Execution**:
  - Verified MegaBot systematically targets entire rows or columns, starting with the least explored.
  - Checked for proper handling of misses and continuation to the next row/column.
- **Expected Behavior**:
  - Torpedo attacks maximize coverage, ensuring MegaBot progresses efficiently.

### **Conclusion**

In this testing report, we focused on testing the main functions that required significant time and effort to implement. These functions represent the bulk of the game’s core logic, such as ship placement, firing mechanics, advanced moves (artillery and torpedo), and the MegaBot strategy.

While we did not provide separate test cases for every auxiliary function, those functions were naturally tested during the execution of these primary functionalities since they are closely connected. For example, functions like board updates, input validation, and ship sinking checks were implicitly validated when running the test cases for `fireAt`, `placeShip`, and `MegaBotMakeMove`.

The test cases we documented in this report comprehensively cover the game’s critical logic and ensure that the program behaves reliably under both normal and edge-case scenarios.


