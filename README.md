# Tic-Tac-Toe Game with Enhanced AI

## ENGG1410: Introductory Programming for Engineers
**Mini Project Part #1**

**Author:** Mohamad Abou El Nasr  
**Institution:** College of Engineering, University of Guelph  
**Date:** November 8, 2025

---

## Project Description

This is a C implementation of the classic Tic-Tac-Toe game with enhanced AI capabilities. The program supports:
- Customizable grid sizes (3x3 to 10x10)
- Two game modes: Player vs Player and Player vs AI
- Strategic AI that can win and block opponent moves
- Score tracking across multiple rounds

---

## Features

### 1. Customizable Grid Size
- Players can choose grid sizes from 3x3 up to 10x10
- Larger grids provide more challenging gameplay

### 2. Two Game Modes
- **Player vs Player:** Two human players alternate turns
- **Player vs AI:** Human player (X) competes against AI opponent (O)

### 3. Enhanced AI Strategy
The AI uses intelligent decision-making:
1. **Win Check:** Takes winning move if available
2. **Block Check:** Blocks opponent's winning move
3. **Center Strategy:** Takes center position when possible
4. **Corner Strategy:** Prioritizes corner positions
5. **Random Move:** Falls back to valid random moves

### 4. Score Tracking
- Tracks wins for Player X and Player O
- Counts draw games
- Displays cumulative scores after each round

### 5. Win/Draw Detection
- Checks rows, columns, and both diagonals for wins
- Detects draw conditions when board is full

---

## How to Compile

Using GCC:
```bash
gcc main.c -o tictactoe
```

Using Visual Studio (Developer Command Prompt):
```bash
cl main.c
```

---

## How to Run

After compilation:
```bash
./tictactoe
```

Or on Windows:
```bash
tictactoe.exe
```

---

## How to Play

1. **Select Grid Size:** Enter a number between 3 and 10
2. **Choose Game Mode:**
   - Enter `1` for Player vs Player
   - Enter `2` for Player vs AI
3. **Make Moves:**
   - Enter row number (0 to size-1)
   - Enter column number (0 to size-1)
4. **Win Condition:** Form a complete line of your mark (horizontal, vertical, or diagonal)
5. **Rematch:** After each game, choose to play again or exit

---

## Program Structure

### Main Components

1. **initializeBoard()** - Sets up empty game board
2. **printBoard()** - Displays current board state with grid
3. **playerMove()** - Handles human player input with validation
4. **aiMove()** - Implements AI strategy and decision-making
5. **checkWin()** - Detects winning conditions
6. **checkDraw()** - Detects draw conditions
7. **updateScore()** - Tracks scores across rounds
8. **canWin()** - Helper function to find winning/blocking moves

### Global Variables
- `playerXScore` - Tracks Player X wins
- `playerOScore` - Tracks Player O wins
- `draws` - Tracks draw games

---

## Input Validation

The program includes robust input validation:
- Grid size must be between 3 and 10
- Game mode must be 1 or 2
- Row and column must be within valid range
- Selected cell must be empty

---

## Sample Gameplay

```
===================================
  TIC-TAC-TOE GAME WITH AI
===================================

Enter grid size (3-10): 3

Select game mode:
1. Player vs Player
2. Player vs AI
Enter your choice (1 or 2): 2

--- Game Start! ---
You are X, AI is O

   0   1   2  
  +---+---+---+
0 |   |   |   |
  +---+---+---+
1 |   |   |   |
  +---+---+---+
2 |   |   |   |
  +---+---+---+

Player X's turn:
Enter row (0-2): 1
Enter column (0-2): 1

AI (O) is thinking...
AI plays at row 0, column 0 (Corner move!)
...
```

---

## AI Strategy Details

### Priority Order:
1. **Winning Move:** If AI can win, it takes the winning position
2. **Blocking Move:** If opponent can win next turn, AI blocks them
3. **Center Control:** Takes center cell if available (odd grids)
4. **Corner Control:** Prioritizes corner positions
5. **Random Valid:** Makes random valid move as fallback

This strategy ensures the AI is competitive while maintaining educational value.

---

## Known Limitations

- AI uses basic strategy suitable for educational purposes
- For very large grids (9x9, 10x10), gameplay may be lengthy
- No file save/load functionality in Part 1

---

## Testing Recommendations

1. Test 3x3 grid with both game modes
2. Test larger grids (5x5, 7x7)
3. Verify AI blocking strategy
4. Verify AI winning strategy
5. Test draw detection
6. Test score tracking across multiple rounds
7. Test input validation with invalid inputs

---

## Future Enhancements (Part 2)

Potential features for Part 2:
- Advanced AI with minimax algorithm
- Difficulty levels
- Save/load game state
- Undo/redo moves
- Game replay functionality
- Tournament mode

---

## Compilation Notes

**Required Compiler:** C compiler (GCC, MinGW, Visual Studio, etc.)  
**C Standard:** C99 or later  
**Libraries:** Standard library only (`stdio.h`, `stdlib.h`, `time.h`)

---

## Troubleshooting

**Issue:** AI always makes random moves  
**Solution:** Ensure `srand(time(NULL))` is called only once in main()

**Issue:** Board display is misaligned  
**Solution:** Check terminal supports grid characters; adjust spacing in printBoard()

**Issue:** Compilation warnings  
**Solution:** Ensure using C99 standard or later (`-std=c99` flag)

---

## Author Information

**Course:** ENGG1410 - Introductory Programming for Engineers  
**Instructor:** Mohamad Abou El Nasr  
**Institution:** University of Guelph, College of Engineering

---

## License

This project is for educational purposes as part of ENGG1410 coursework.
