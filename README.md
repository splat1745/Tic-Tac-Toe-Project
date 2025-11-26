# TIC-TAC-TOE - Advanced Implementation

## Overview
This is the Part 2 implementation of the Tic-Tac-Toe project with advanced features including dynamic memory allocation (DMA), file I/O, and enhanced AI with difficulty levels.

## Files
- **mainp2_part2.c** - Complete Part 2 implementation with all features

## Features Implemented

### 1. Dynamic Board Allocation (Core Enhancement #1)
- **createBoard(int size)** - Allocates a 2D dynamic array using char** (double pointer)
- **freeBoard(char** board, int size)** - Properly deallocates all memory
- Supports board sizes from 3×3 to 15×15
- No memory leaks when freeing the board

**Key Concept:** The board is represented as `char**` (pointer to pointers), where each row is allocated separately:
```c
board = malloc(size * sizeof(char*));  // Array of row pointers
for (i = 0; i < size; i++) {
    board[i] = malloc(size * sizeof(char));  // Each row
}
```

### 2. Game Statistics with Dynamic Structures (Core Enhancement #2)
- **GameStats** typedef tracks:
  - games_played
  - wins_player1 (Player X)
  - wins_player2 (Player O)
  - draws
  - **win_patterns** - Dynamic array that grows as needed using realloc

- **createGameStats()** - Allocates and initializes the statistics tracker
- **updateStats()** - Updates stats and resizes win_patterns array dynamically
- **freeGameStats()** - Safely deallocates all memory
- **printStatistics()** - Displays game statistics

**Key Concept:** The win_patterns array demonstrates dynamic resizing with realloc:
```c
if (stats->pattern_count >= stats->pattern_capacity) {
    stats->pattern_capacity *= 2;  // Double capacity
    new_patterns = realloc(stats->win_patterns, 
                          stats->pattern_capacity * sizeof(int));
}
```

### 3. File I/O System (Core Enhancement #3)
- **saveGame(char** board, int size, const GameStats* stats, const char* filename)**
  - Saves board state and game statistics to a text file
  - Format: board size, then each row, then statistics

- **loadGame(char*** board, int* size, GameStats* stats, const char* filename)**
  - Loads a previously saved game
  - Note: Takes char*** because board is allocated inside the function
  - Demonstrates pointer-to-pointer usage

- **saveStatistics(const GameStats* stats, const char* filename)**
  - Saves only statistics in a human-readable format

**Usage in the game:**
1. After a game ends, user is asked "Save game? (y/n)"
2. Saves to "savegame.txt" and "statistics.txt"
3. Main menu option allows loading previous games

### 4. Enhanced AI with Difficulty Levels (Advanced Feature A)
- **EASY** - Pure random move selection
- **MEDIUM** - Rule-based heuristics (try to win, block opponent, center, corners)
- **HARD** - Minimax algorithm with game tree evaluation

#### Minimax Algorithm (HARD Mode)
- **minimax(char** board, int size, char ai_char, char player_char, int depth, bool is_maximizing)**
  - Recursive minimax evaluation
  - Maximizing player (AI) tries to maximize score
  - Minimizing player (human) tries to minimize score
  - Returns best move (row, col) and its score

- **evaluateBoard()** - Heuristic evaluation at depth limit to control computation time
  - Counts potential winning lines for each player
  - Prevents excessive recursion on large boards

**Minimax Flow:**
1. Check terminal states (win/loss/draw)
2. If depth limit reached, use heuristic evaluation
3. Try all possible moves
4. Recursively evaluate resulting positions
5. Choose move with best minimax score

## Compilation

```bash
gcc -o mainp2_part2 mainp2_part2.c -std=c99 -Wall -Wextra
```

## Running the Program

```bash
./mainp2_part2.exe
```

### Main Menu Options
1. **New Game**
   - Enter board size (3-15)
   - Select game mode (PvP or PvAI)
   - If PvAI, select difficulty (EASY/MEDIUM/HARD)
   - Play the game
   - Option to save after game ends

2. **Load Game**
   - Loads the last saved game from "savegame.txt"
   - Displays the board state

3. **View Statistics**
   - Displays accumulated game statistics
   - Games played, wins, draws, etc.

4. **Exit**
   - Shows final statistics before quitting

## Memory Management

All dynamic memory is properly allocated and freed:
- ✅ No memory leaks
- ✅ Dynamic board allocation with char**
- ✅ Dynamic win_patterns array with realloc
- ✅ Proper cleanup in all functions
- ✅ Error handling for allocation failures

## Game Loop Integration

The Part 2 game loop:
1. Creates dynamic board using createBoard()
2. Initializes GameStats structure
3. Plays game with chosen difficulty AI
4. Updates statistics with updateStats()
5. Optionally saves game and statistics
6. Frees board and statistics before returning to menu

## Key Improvements Over Part 1

| Feature | Part 1 | Part 2 |
|---------|--------|--------|
| Board Size | Fixed 10×10 | Dynamic 3-15×15 |
| Memory | Static array | Dynamic allocation (char**) |
| Statistics | Global counters | GameStats structure |
| Persistence | None | Save/Load to files |
| AI Difficulty | 1 level (MEDIUM) | 3 levels (EASY/MEDIUM/HARD) |
| AI Algorithm | Heuristics | Minimax (HARD) |

## Educational Value

This implementation teaches:
1. **Dynamic Memory Allocation** - malloc, free, realloc
2. **Pointer Arithmetic** - char**, pointer-to-pointers
3. **Data Structures** - typedef struct with dynamic arrays
4. **File I/O** - fopen, fprintf, fscanf, fclose
5. **Recursive Algorithms** - Minimax with game tree search
6. **Software Engineering** - Separation of concerns, error handling

## Notes

- The depth limit in minimax (4) prevents excessive computation on larger boards
- Random seed uses time(NULL) for variety
- Input validation prevents out-of-bounds access
- All file operations check for success and handle errors

## Future Enhancements

- Add alpha-beta pruning to minimax for faster evaluation
- Implement game replay system (store move history)
- Add difficulty settings for MEDIUM AI (more heuristics)
- Persistent leaderboard across sessions
- Graphical UI instead of console
