// engg1410: introductory programming for engineers
// mini project part#1: tic-tac-toe game simulation with enhanced ai
// mohamad abou el nasr
// college of engineering, university of guelph
// november 8, 2025
//
// detailed comments added by github copilot (your assistant) to explain
// implementation choices, data layout, and individual lines of code.
// the goal of these comments is educational: to explain why particular
// design choices were made (e.g., global constants, array layout, simple
// ai heuristics) and to explain more subtle or non-obvious lines.

// include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10 // maximum grid size

// global variables for score tracking
int playerXScore = 0;    // tracks wins for player x
int playerOScore = 0;    // tracks wins for player o
int draws = 0;           // tracks number of draw games

// function prototypes
// function prototypes with explanatory notes
// ------------------------------------------------------------------
// prototypes (forward declarations) are used so the order of the
// functions in the file doesn't matter. each function is described in
// detail where it is implemented, but here's a quick map of what each
// function does and why it exists:
// - initializeBoard: fills the 2D array with spaces to mark empty cells.
// - printBoard: prints a nicely formatted grid. useful separation of
//               concerns (display vs. game logic).
// - playerMove: prompts the user for a move and validates input.
// - aiMove: simple rule-based ai (tries to win, blocks opponent, takes
//           center/corners, otherwise random) — good teaching example
// - canWin: checks if a player can win on the next move; used by ai.
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
    
    // seed random number generator with current time
    // ensures different random moves each game run
    srand(time(NULL));
    
    printf("===================================\n");
    printf("  TIC-TAC-TOE GAME WITH AI (somewhat anyway)\n");
    printf("===================================\n\n");
    
    do {
        // get board size from user with validation loop
        do {
            printf("Enter grid size (3-10): ");      // prompt user for board size
            scanf("%d", &size);                       // read the size
            if (size < 3 || size > MAX_SIZE) {        // check if size is valid (3-10)
                printf("Invalid size! Please enter a value between 3 and 10.\n");
            }
        } while (size < 3 || size > MAX_SIZE);        // repeat until valid input
        
        // get game mode selection with validation loop
        do {
            printf("\nSelect game mode:\n");          // display game mode menu
            printf("1. Player vs Player\n");           // option 1: pvp
            printf("2. Player vs AI\n");                // option 2: pvai
            printf("Enter your choice (1 or 2): ");    // prompt for choice
            scanf("%d", &gameMode);                    // read the choice
            if (gameMode != 1 && gameMode != 2) {      // validate the input
                printf("Invalid choice! Please enter 1 or 2.\n");
            }
        } while (gameMode != 1 && gameMode != 2);      // repeat until valid
        
        // fill all board cells with space characters
        initializeBoard(board, size);
        
        // initialize game state: x plays first, game is not over
        currentPlayer = 'X';  // set x as the starting player
        gameOver = 0;         // game has not ended yet
        
        printf("\n--- Game Start! ---\n");
        if (gameMode == 2) {
            printf("You are X, AI is O\n");
        }
        printf("\n");
        
        // main game loop - continues until someone wins or draw occurs
        while (!gameOver) {
            // display current board state
            printBoard(board, size);
            
            // determine whose turn it is and get their move
            if (gameMode == 1 || currentPlayer == 'X') {
                // human player turn (in pvp, always human; in pvai, x is human)
                printf("\nPlayer %c's turn:\n", currentPlayer);  // announce player
                playerMove(board, size, currentPlayer);          // get their move
            } else {
                // ai turn (only in pvai mode when o's turn)
                printf("\nAI (O) is thinking...\n");              // announce ai
                aiMove(board, size, 'O');                        // ai plays
            }
            
            // check if current player has won
            if (checkWin(board, size, currentPlayer)) {
                printBoard(board, size);                         // show final board
                printf("\n*** Player %c wins! ***\n\n", currentPlayer);
                updateScore(currentPlayer);                      // increment winner's score
                gameOver = 1;                                    // end the game
            }
            // check if game is a draw
            else if (checkDraw(board, size)) {
                printBoard(board, size);                         // show final board
                printf("\n*** It's a draw! ***\n\n");
                updateScore('D');                                // increment draw count
                gameOver = 1;                                    // end the game
            }
            // game continues: switch to other player
            else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';  // toggle player
            }
        }
        
        // display accumulated scores
        printf("===================================\n");
        printf("         SCORE BOARD\n");         // title
        printf("===================================\n");
        printf("Player X: %d\n", playerXScore);  // x's total wins
        printf("Player O: %d\n", playerOScore);  // o's total wins
        printf("Draws:    %d\n", draws);         // total draws
        printf("===================================\n\n");
        
        // ask if player wants another game
        printf("Play again? (y/n): ");  // prompt for yes/no
        scanf(" %c", &playAgain);    // read single character response
        printf("\n");
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("Thank you for playing!\n");
    printf("Final Scores - X: %d, O: %d, Draws: %d\n", playerXScore, playerOScore, draws);
    
    return 0;
}

// initialize board by filling all cells with spaces
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;

    // iterate through all rows
    for (i = 0; i < size; i++) {
        // iterate through all columns in each row
        for (j = 0; j < size; j++) {
            // mark cell as empty using space character
            board[i][j] = ' ';
        }
    }
}

// display the current state of the board
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    printf("\n");
    
    // print column header with indices (0, 1, 2, ...)
    printf("   ");          // space for row labels
    for (j = 0; j < size; j++) {
        printf(" %d  ", j); // print each column number
    }
    printf("\n");           // newline after column headers
    
    // draw top border of the grid
    printf("  ");          // space for row labels
    for (j = 0; j < size; j++) {
        printf("+---");     // horizontal segment
    }
    printf("+\n");         // final corner and newline
    
    // print each row with cell contents and separators
    for (i = 0; i < size; i++) {
        printf("%d ", i);                    // print row number
        for (j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);   // print cell content
        }
        printf("|\n");                       // end of row
        
        // draw horizontal separator below current row
        printf("  ");                        // space for row labels
        for (j = 0; j < size; j++) {
            printf("+---");                 // horizontal segment
        }
        printf("+\n");                       // final corner and newline
    }
    printf("\n");
}

// get a valid move from the player with input validation
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int row, col;
    int validMove = 0;  // flag to exit validation loop
    
    // keep asking until valid move is made
    while (!validMove) {
        printf("Enter row (0-%d): ", size - 1);      // prompt for row
        scanf("%d", &row);                            // read row input
        printf("Enter column (0-%d): ", size - 1);  // prompt for column
        scanf("%d", &col);                            // read column input
        
        // check if row/column are within bounds
        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Invalid input! Row and column must be between 0 and %d.\n", size - 1);
        } 
        // check if cell is already occupied
        else if (board[row][col] != ' ') {
            printf("Cell already occupied! Choose another cell.\n");
        } 
        // valid move: place player's mark and exit loop
        else {
            board[row][col] = player;  // place the player's mark
            validMove = 1;              // exit the validation loop
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
    
    // check all rows for near-complete lines
    for (i = 0; i < size; i++) {
        count = 0;           // count player's marks in this row
        emptyRow = -1;       // track location of empty cell
        emptyCol = -1;
        
        for (j = 0; j < size; j++) {
            if (board[i][j] == player) {
                count++;     // player has a mark here
            } else if (board[i][j] == ' ') {
                emptyRow = i;  // remember empty cell position
                emptyCol = j;
            }
        }
        
        // if player has size-1 marks and one empty cell: winning move!
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;   // return winning position
            *col = emptyCol;
            return 1;          // found a winning opportunity
        }
    }
    
    // check all columns using same logic as rows
    for (j = 0; j < size; j++) {
        count = 0;           // reset counter for each column
        emptyRow = -1;       // reset empty cell tracker
        emptyCol = -1;
        
        for (i = 0; i < size; i++) {
            if (board[i][j] == player) {
                count++;     // player has a mark in this column
            } else if (board[i][j] == ' ') {
                emptyRow = i;  // track empty cell in this column
                emptyCol = j;
            }
        }
        
        // if player has size-1 marks and one empty: winning move!
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;   // return the winning position
            *col = emptyCol;
            return 1;          // found a winning opportunity
        }
    }
    
    // check main diagonal (top-left to bottom-right)
    count = 0;             // reset counter
    emptyRow = -1;         // reset empty cell tracker
    emptyCol = -1;
    
    for (i = 0; i < size; i++) {
        if (board[i][i] == player) {
            count++;       // player has mark on diagonal at (i,i)
        } else if (board[i][i] == ' ') {
            emptyRow = i;  // track empty cell on diagonal
            emptyCol = i;
        }
    }
    
    // check if diagonal has size-1 marks with one empty cell
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;   // return winning position
        *col = emptyCol;
        return 1;          // found a winning move
    }
    
    // check anti-diagonal (top-right to bottom-left)
    count = 0;                    // reset counter
    emptyRow = -1;                // reset empty cell tracker
    emptyCol = -1;
    
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] == player) {
            count++;               // player has mark on anti-diagonal
        } else if (board[i][size - 1 - i] == ' ') {
            emptyRow = i;          // track empty cell on anti-diagonal
            emptyCol = size - 1 - i;
        }
    }
    
    // check if anti-diagonal has size-1 marks with one empty cell
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;           // return winning position
        *col = emptyCol;
        return 1;                  // found a winning move
    }
    
    return 0;
}

// enhanced ai move with strategic decision-making
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer) {
    int row, col;
    char opponent = (aiPlayer == 'X') ? 'O' : 'X';
    
    // strategy 1: try to win immediately
    if (canWin(board, size, aiPlayer, &row, &col)) {
        board[row][col] = aiPlayer;                  // place winning move
        printf("AI plays at row %d, column %d (Winning move!)\n", row, col);
        return;                                      // move complete
    }
    
    // strategy 2: block opponent's winning move
    if (canWin(board, size, opponent, &row, &col)) {
        board[row][col] = aiPlayer;                  // block the threat
        printf("AI plays at row %d, column %d (Blocking move!)\n", row, col);
        return;                                      // move complete
    }
    
    // strategy 3: take center if available (strong position)
    if (size % 2 == 1) {                               // only for odd-sized boards
        int center = size / 2;                         // calculate center position
        if (isCellEmpty(board, center, center)) {
            board[center][center] = aiPlayer;          // place mark at center
            printf("AI plays at row %d, column %d (Center move!)\n", center, center);
            return;                                    // move complete
        }
    }
    
    // strategy 4: take a corner (strategic positions)
    int corners[4][2] = {{0, 0}, {0, size-1}, {size-1, 0}, {size-1, size-1}};
    for (int i = 0; i < 4; i++) {
        if (isCellEmpty(board, corners[i][0], corners[i][1])) {
            board[corners[i][0]][corners[i][1]] = aiPlayer;  // place at corner
            printf("AI plays at row %d, column %d (Corner move!)\n", corners[i][0], corners[i][1]);
            return;                                    // move complete
        }
    }
    
    // strategy 5: pick random empty cell (fallback)
    do {
        row = rand() % size;      // generate random row
        col = rand() % size;      // generate random column
    } while (!isCellEmpty(board, row, col));  // repeat until empty cell found
    
    board[row][col] = aiPlayer;   // place mark at random position
    printf("AI plays at row %d, column %d\n", row, col);
}

// check if a player has won
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int i, j;
    int win;
    
    // check all rows for complete win
    for (i = 0; i < size; i++) {
        win = 1;                                // assume row is a win
        for (j = 0; j < size; j++) {
            if (board[i][j] != player) {
                win = 0;                       // found non-player mark
                break;                         // stop checking this row
            }
        }
        if (win) return 1;                     // found winning row!
    }
    
    // check all columns for complete win
    for (j = 0; j < size; j++) {
        win = 1;                                // assume column is a win
        for (i = 0; i < size; i++) {
            if (board[i][j] != player) {
                win = 0;                       // found non-player mark
                break;                         // stop checking this column
            }
        }
        if (win) return 1;                     // found winning column!
    }
    
    // check main diagonal (top-left to bottom-right)
    win = 1;                                   // assume diagonal is a win
    for (i = 0; i < size; i++) {
        if (board[i][i] != player) {
            win = 0;                           // found non-player mark
            break;                             // stop checking
        }
    }
    if (win) return 1;                         // found winning diagonal!
    
    // check anti-diagonal (top-right to bottom-left)
    win = 1;                                   // assume diagonal is a win
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != player) {
            win = 0;                           // found non-player mark
            break;                             // stop checking
        }
    }
    if (win) return 1;                         // found winning anti-diagonal!
    
    return 0;                                  // no winning line found
}

// check if the game is a draw (board is full)
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    // scan all cells for empty spaces
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                return 0;  // found empty cell, game can continue
            }
        }
    }
    
    return 1;              // board is full, it's a draw
}

// update score based on game outcome
void updateScore(char winner) {
    if (winner == 'X') {
        playerXScore++;  // increment player x's wins
    } else if (winner == 'O') {
        playerOScore++;  // increment player o's wins
    } else if (winner == 'D') {
        draws++;         // increment draw count
    }
}
