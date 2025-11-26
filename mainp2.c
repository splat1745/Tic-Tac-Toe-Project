// engg1410: introductory programming for engineers
// mini project part#1: tic-tac-toe game simulation with enhanced ai
// mohamad abou el nasr
// college of engineering, university of guelph
// november 8, 2025
//
// Detailed comments added by GitHub Copilot (your assistant) to explain
// implementation choices, data layout, and individual lines of code.
// The goal of these comments is educational: to explain why particular
// design choices were made (e.g., global constants, array layout, simple
// AI heuristics) and to explain more subtle or non-obvious lines.

// include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10 // maximum grid size

// Global variables for score tracking
int playerXScore = 0;    // Tracks wins for player X
int playerOScore = 0;    // Tracks wins for player O
int draws = 0;           // Tracks number of draw games

// function prototypes
// Function prototypes with explanatory notes
// ------------------------------------------------------------------
// Prototypes (forward declarations) are used so the order of the
// functions in the file doesn't matter. Each function is described in
// detail where it is implemented, but here's a quick map of what each
// function does and why it exists:
// - initializeBoard: fills the 2D array with spaces to mark empty cells.
// - printBoard: prints a nicely formatted grid. Useful separation of
//               concerns (display vs. game logic).
// - playerMove: prompts the user for a move and validates input.
// - aiMove: simple rule-based AI (tries to win, blocks opponent, takes
//           center/corners, otherwise random) — good teaching example
// - canWin: checks if a player can win on the next move; used by AI.
// - isCellEmpty: helper to test whether a cell is unoccupied.
// - checkWin/checkDraw: terminal checks used to determine game state.
// - updateScore: increments the appropriate global score counter.
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player);
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer);
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player);
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size);
void updateScore(char winner);
int canWin(char board[MAX_SIZE][MAX_SIZE], int size, char player, int *row, int *col);
int isCellEmpty(char board[MAX_SIZE][MAX_SIZE], int row, int col);

// main function
int main() {
    char board[MAX_SIZE][MAX_SIZE];
    int size;
    int gameMode;
    char currentPlayer;
    int gameOver;
    char playAgain;
    
    // Seed random number generator with current time
    // Ensures different random moves each game run
    srand(time(NULL));
    
    printf("===================================\n");
    printf("  TIC-TAC-TOE GAME WITH AI (somewhat anyway)\n");
    printf("===================================\n\n");
    
    do {
        // Get board size from user with validation loop
        do {
            printf("Enter grid size (3-10): ");      // Prompt user for board size
            scanf("%d", &size);                       // Read the size
            if (size < 3 || size > MAX_SIZE) {        // Check if size is valid (3-10)
                printf("Invalid size! Please enter a value between 3 and 10.\n");
            }
        } while (size < 3 || size > MAX_SIZE);        // Repeat until valid input
        
        // Get game mode selection with validation loop
        do {
            printf("\nSelect game mode:\n");          // Display game mode menu
            printf("1. Player vs Player\n");           // Option 1: PvP
            printf("2. Player vs AI\n");                // Option 2: PvAI
            printf("Enter your choice (1 or 2): ");    // Prompt for choice
            scanf("%d", &gameMode);                    // Read the choice
            if (gameMode != 1 && gameMode != 2) {      // Validate the input
                printf("Invalid choice! Please enter 1 or 2.\n");
            }
        } while (gameMode != 1 && gameMode != 2);      // Repeat until valid
        
        // Fill all board cells with space characters
        initializeBoard(board, size);
        
        // Initialize game state: X plays first, game is not over
        currentPlayer = 'X';  // Set X as the starting player
        gameOver = 0;         // Game has not ended yet
        
        printf("\n--- Game Start! ---\n");
        if (gameMode == 2) {
            printf("You are X, AI is O\n");
        }
        printf("\n");
        
        // Main game loop - continues until someone wins or draw occurs
        while (!gameOver) {
            // Display current board state
            printBoard(board, size);
            
            // Determine whose turn it is and get their move
            if (gameMode == 1 || currentPlayer == 'X') {
                // Human player turn (in PvP, always human; in PvAI, X is human)
                printf("\nPlayer %c's turn:\n", currentPlayer);  // Announce player
                playerMove(board, size, currentPlayer);          // Get their move
            } else {
                // AI turn (only in PvAI mode when O's turn)
                printf("\nAI (O) is thinking...\n");              // Announce AI
                aiMove(board, size, 'O');                        // AI plays
            }
            
            // Check if current player has won
            if (checkWin(board, size, currentPlayer)) {
                printBoard(board, size);                         // Show final board
                printf("\n*** Player %c wins! ***\n\n", currentPlayer);
                updateScore(currentPlayer);                      // Increment winner's score
                gameOver = 1;                                    // End the game
            }
            // Check if game is a draw
            else if (checkDraw(board, size)) {
                printBoard(board, size);                         // Show final board
                printf("\n*** It's a draw! ***\n\n");
                updateScore('D');                                // Increment draw count
                gameOver = 1;                                    // End the game
            }
            // Game continues: switch to other player
            else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';  // Toggle player
            }
        }
        
        // Display accumulated scores
        printf("===================================\n");
        printf("         SCORE BOARD\n");         // Title
        printf("===================================\n");
        printf("Player X: %d\n", playerXScore);  // X's total wins
        printf("Player O: %d\n", playerOScore);  // O's total wins
        printf("Draws:    %d\n", draws);         // Total draws
        printf("===================================\n\n");
        
        // Ask if player wants another game
        printf("Play again? (y/n): ");  // Prompt for yes/no
        scanf(" %c", &playAgain);    // Read single character response
        printf("\n");
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("Thank you for playing!\n");
    printf("Final Scores - X: %d, O: %d, Draws: %d\n", playerXScore, playerOScore, draws);
    
    return 0;
}

// Initialize board by filling all cells with spaces
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;

    // Iterate through all rows
    for (i = 0; i < size; i++) {
        // Iterate through all columns in each row
        for (j = 0; j < size; j++) {
            // Mark cell as empty using space character
            board[i][j] = ' ';
        }
    }
}

// display the current state of the board
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    printf("\n");
    
    // Print column header with indices (0, 1, 2, ...)
    printf("   ");          // Space for row labels
    for (j = 0; j < size; j++) {
        printf(" %d  ", j); // Print each column number
    }
    printf("\n");           // Newline after column headers
    
    // Draw top border of the grid
    printf("  ");          // Space for row labels
    for (j = 0; j < size; j++) {
        printf("+---");     // Horizontal segment
    }
    printf("+\n");         // Final corner and newline
    
    // Print each row with cell contents and separators
    for (i = 0; i < size; i++) {
        printf("%d ", i);                    // Print row number
        for (j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);   // Print cell content
        }
        printf("|\n");                       // End of row
        
        // Draw horizontal separator below current row
        printf("  ");                        // Space for row labels
        for (j = 0; j < size; j++) {
            printf("+---");                 // Horizontal segment
        }
        printf("+\n");                       // Final corner and newline
    }
    printf("\n");
}

// Get a valid move from the player with input validation
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int row, col;
    int validMove = 0;  // Flag to exit validation loop
    
    // Keep asking until valid move is made
    while (!validMove) {
        printf("Enter row (0-%d): ", size - 1);      // Prompt for row
        scanf("%d", &row);                            // Read row input
        printf("Enter column (0-%d): ", size - 1);  // Prompt for column
        scanf("%d", &col);                            // Read column input
        
        // Check if row/column are within bounds
        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Invalid input! Row and column must be between 0 and %d.\n", size - 1);
        } 
        // Check if cell is already occupied
        else if (board[row][col] != ' ') {
            printf("Cell already occupied! Choose another cell.\n");
        } 
        // Valid move: place player's mark and exit loop
        else {
            board[row][col] = player;  // Place the player's mark
            validMove = 1;              // Exit the validation loop
        }
    }
}

// check if a cell is empty
int isCellEmpty(char board[MAX_SIZE][MAX_SIZE], int row, int col) {
    return board[row][col] == ' ';
}

// check if a player can win in the next move
// returns 1 if win is possible and sets row and col to winning position
int canWin(char board[MAX_SIZE][MAX_SIZE], int size, char player, int *row, int *col) {
    int i, j;
    int count, emptyRow, emptyCol;
    
    // Check all rows for near-complete lines
    for (i = 0; i < size; i++) {
        count = 0;           // Count player's marks in this row
        emptyRow = -1;       // Track location of empty cell
        emptyCol = -1;
        
        for (j = 0; j < size; j++) {
            if (board[i][j] == player) {
                count++;     // Player has a mark here
            } else if (board[i][j] == ' ') {
                emptyRow = i;  // Remember empty cell position
                emptyCol = j;
            }
        }
        
        // If player has size-1 marks and one empty cell: winning move!
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;   // Return winning position
            *col = emptyCol;
            return 1;          // Found a winning opportunity
        }
    }
    
    // Check all columns using same logic as rows
    for (j = 0; j < size; j++) {
        count = 0;           // Reset counter for each column
        emptyRow = -1;       // Reset empty cell tracker
        emptyCol = -1;
        
        for (i = 0; i < size; i++) {
            if (board[i][j] == player) {
                count++;     // Player has a mark in this column
            } else if (board[i][j] == ' ') {
                emptyRow = i;  // Track empty cell in this column
                emptyCol = j;
            }
        }
        
        // If player has size-1 marks and one empty: winning move!
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;   // Return the winning position
            *col = emptyCol;
            return 1;          // Found a winning opportunity
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    count = 0;             // Reset counter
    emptyRow = -1;         // Reset empty cell tracker
    emptyCol = -1;
    
    for (i = 0; i < size; i++) {
        if (board[i][i] == player) {
            count++;       // Player has mark on diagonal at (i,i)
        } else if (board[i][i] == ' ') {
            emptyRow = i;  // Track empty cell on diagonal
            emptyCol = i;
        }
    }
    
    // Check if diagonal has size-1 marks with one empty cell
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;   // Return winning position
        *col = emptyCol;
        return 1;          // Found a winning move
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    count = 0;                    // Reset counter
    emptyRow = -1;                // Reset empty cell tracker
    emptyCol = -1;
    
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] == player) {
            count++;               // Player has mark on anti-diagonal
        } else if (board[i][size - 1 - i] == ' ') {
            emptyRow = i;          // Track empty cell on anti-diagonal
            emptyCol = size - 1 - i;
        }
    }
    
    // Check if anti-diagonal has size-1 marks with one empty cell
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;           // Return winning position
        *col = emptyCol;
        return 1;                  // Found a winning move
    }
    
    return 0;
}

// enhanced ai move with strategic decision-making
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer) {
    int row, col;
    char opponent = (aiPlayer == 'X') ? 'O' : 'X';
    
    // Strategy 1: Try to win immediately
    if (canWin(board, size, aiPlayer, &row, &col)) {
        board[row][col] = aiPlayer;                  // Place winning move
        printf("AI plays at row %d, column %d (Winning move!)\n", row, col);
        return;                                      // Move complete
    }
    
    // Strategy 2: Block opponent's winning move
    if (canWin(board, size, opponent, &row, &col)) {
        board[row][col] = aiPlayer;                  // Block the threat
        printf("AI plays at row %d, column %d (Blocking move!)\n", row, col);
        return;                                      // Move complete
    }
    
    // Strategy 3: Take center if available (strong position)
    if (size % 2 == 1) {                               // Only for odd-sized boards
        int center = size / 2;                         // Calculate center position
        if (isCellEmpty(board, center, center)) {
            board[center][center] = aiPlayer;          // Place mark at center
            printf("AI plays at row %d, column %d (Center move!)\n", center, center);
            return;                                    // Move complete
        }
    }
    
    // Strategy 4: Take a corner (strategic positions)
    int corners[4][2] = {{0, 0}, {0, size-1}, {size-1, 0}, {size-1, size-1}};
    for (int i = 0; i < 4; i++) {
        if (isCellEmpty(board, corners[i][0], corners[i][1])) {
            board[corners[i][0]][corners[i][1]] = aiPlayer;  // Place at corner
            printf("AI plays at row %d, column %d (Corner move!)\n", corners[i][0], corners[i][1]);
            return;                                    // Move complete
        }
    }
    
    // Strategy 5: Pick random empty cell (fallback)
    do {
        row = rand() % size;      // Generate random row
        col = rand() % size;      // Generate random column
    } while (!isCellEmpty(board, row, col));  // Repeat until empty cell found
    
    board[row][col] = aiPlayer;   // Place mark at random position
    printf("AI plays at row %d, column %d\n", row, col);
}

// check if a player has won
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int i, j;
    int win;
    
    // Check all rows for complete win
    for (i = 0; i < size; i++) {
        win = 1;                                // Assume row is a win
        for (j = 0; j < size; j++) {
            if (board[i][j] != player) {
                win = 0;                       // Found non-player mark
                break;                         // Stop checking this row
            }
        }
        if (win) return 1;                     // Found winning row!
    }
    
    // Check all columns for complete win
    for (j = 0; j < size; j++) {
        win = 1;                                // Assume column is a win
        for (i = 0; i < size; i++) {
            if (board[i][j] != player) {
                win = 0;                       // Found non-player mark
                break;                         // Stop checking this column
            }
        }
        if (win) return 1;                     // Found winning column!
    }
    
    // Check main diagonal (top-left to bottom-right)
    win = 1;                                   // Assume diagonal is a win
    for (i = 0; i < size; i++) {
        if (board[i][i] != player) {
            win = 0;                           // Found non-player mark
            break;                             // Stop checking
        }
    }
    if (win) return 1;                         // Found winning diagonal!
    
    // Check anti-diagonal (top-right to bottom-left)
    win = 1;                                   // Assume diagonal is a win
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != player) {
            win = 0;                           // Found non-player mark
            break;                             // Stop checking
        }
    }
    if (win) return 1;                         // Found winning anti-diagonal!
    
    return 0;                                  // No winning line found
}

// Check if the game is a draw (board is full)
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    // Scan all cells for empty spaces
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                return 0;  // Found empty cell, game can continue
            }
        }
    }
    
    return 1;              // Board is full, it's a draw
}

// Update score based on game outcome
void updateScore(char winner) {
    if (winner == 'X') {
        playerXScore++;  // Increment player X's wins
    } else if (winner == 'O') {
        playerOScore++;  // Increment player O's wins
    } else if (winner == 'D') {
        draws++;         // Increment draw count
    }
}
