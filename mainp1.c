// engg1410: introductory programming for engineers
// mini project part#1: tic-tac-toe game simulation with enhanced ai
// week 1: basic operation only
// mohamad abou el nasr
// college of engineering, university of guelph
// november 12, 2025
//
// description: this program implements a basic tic-tac-toe game with two-player and player-vs-ai (random) modes
// week 1 focuses on core functionality including board display, input validation, win/draw detection, and score tracking
// ai strategy (winning and blocking moves) will be enhanced in week 2

// include necessary headers
#include <stdio.h>  // for input/output operations (printf, scanf)
#include <stdlib.h> // for standard library functions (rand, srand, malloc)
#include <time.h>   // for time-based random seed (time function)

#define MAX_SIZE 10 // maximum grid size constant (allows grids up to 10x10)

// global variables for score tracking across multiple game rounds
// these maintain cumulative scores throughout the entire session
int playerXScore = 0; // tracks total wins for player x
int playerOScore = 0; // tracks total wins for player o
int draws = 0;        // tracks total draw games

// ==================== function prototypes - week 1 basic functionality ====================
// function declarations allow main() to call these functions before their definitions appear
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player);
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer);
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player);
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size);
void updateScore(char winner);

// ==================== main function ====================
// purpose: serves as the entry point and main control flow of the program
// responsibility: initialize game board and variables
// responsibility: control the main game loop
// responsibility: handle player input for game setup
// responsibility: coordinate between player moves, ai moves, and win/draw detection
// responsibility: manage score tracking and rematch functionality
int main() {
    // local variables declaration for game state management
    char board[MAX_SIZE][MAX_SIZE]; // 2d array representing the game board
    int size;          // user-selected grid size (3-10)
    int gameMode;      // selected game mode (1: pvp, 2: pva)
    char currentPlayer; // tracks whose turn it is ('x' or 'o')
    int gameOver;      // boolean flag: 1 if game ended, 0 if ongoing
    char playAgain;    // user input for rematch (y/n)
    
    // seed random number generator using current time
    // this ensures different random ai moves on each program run
    srand(time(NULL));
    
    printf("===================================\n");
    printf("  TIC-TAC-TOE GAME - WEEK 1\n");
    printf("  (Basic Operation)\n");
    printf("===================================\n\n");
    
    // outer do-while loop: allows multiple game rounds
    // continues until user chooses not to play again
    do {
        // inner do-while loop: validates grid size input
        // ensures size is within acceptable range (3-10)
        do {
            printf("Enter grid size (3-10): ");
            scanf("%d", &size); // read integer from user
            // check if size is outside valid range
            if (size < 3 || size > MAX_SIZE) {
                printf("Invalid size! Please enter a value between 3 and 10.\n");
            }
        } while (size < 3 || size > MAX_SIZE); // repeat until valid input
        
        // nested do-while loop: validates game mode selection
        // ensures user selects either 1 (pvp) or 2 (pva)
        do {
            printf("\nSelect game mode:\n");
            printf("1. Player vs Player\n");
            printf("2. Player vs AI (Basic - Random moves only)\n");
            printf("Enter your choice (1 or 2): ");
            scanf("%d", &gameMode); // read mode choice
            // validate that choice is either 1 or 2
            if (gameMode != 1 && gameMode != 2) {
                printf("Invalid choice! Please enter 1 or 2.\n");
            }
        } while (gameMode != 1 && gameMode != 2); // repeat until valid input
        
        // initialize a fresh board with all empty cells
        // must do this before each new game
        initializeBoard(board, size);
        
        // set starting player to x
        // x always goes first in tic-tac-toe
        currentPlayer = 'X';
        // initialize game state flag: 0 = game ongoing, 1 = game ended
        gameOver = 0;
        
        // display game start message
        printf("\n--- Game Start! ---\n");
        // provide ai information if playing against ai
        if (gameMode == 2) {
            printf("You are X, AI is O (making random moves)\n");
            printf("Note: AI strategy will be added in Week 2\n");
        }
        printf("\n");
        
        // main game loop: continues until someone wins or draw occurs
        while (!gameOver) {
            // display current board state to players
            printBoard(board, size);
            
            // determine whose turn it is and execute appropriate move
            // if pvp mode or if it's player x's turn: human makes move
            if (gameMode == 1 || currentPlayer == 'X') {
                // human player's turn
                printf("\nPlayer %c's turn:\n", currentPlayer);
                // get validated move input from human player
                playerMove(board, size, currentPlayer);
            } else {
                // ai player's turn (player o in pva mode)
                printf("\nAI (O) is thinking...\n");
                // execute ai move (random in week 1, strategic in week 2)
                aiMove(board, size, 'O');
            }
            
            // check if current player's last move resulted in a win
            // checkwin returns 1 if winning condition met, 0 otherwise
            if (checkWin(board, size, currentPlayer)) {
                // display final board state
                printBoard(board, size);
                // announce winner
                printf("\n*** Player %c wins! ***\n\n", currentPlayer);
                // update global score counters
                updateScore(currentPlayer);
                // set flag to end game loop
                gameOver = 1;
            }
            // if no win, check if game board is completely filled (draw)
            else if (checkDraw(board, size)) {
                // display final board state
                printBoard(board, size);
                // announce draw
                printf("\n*** It's a draw! ***\n\n");
                // update draw counter in global scores
                updateScore('D');
                // set flag to end game loop
                gameOver = 1;
            }
            // if no win or draw, alternate to other player
            else {
                // ternary operator: switch x to o or o to x
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }
        
        // display current scores after game ends
        printf("===================================\n");
        printf("         SCORE BOARD\n");
        printf("===================================\n");
        printf("Player X: %d\n", playerXScore); // show x's cumulative wins
        printf("Player O: %d\n", playerOScore); // show o's cumulative wins
        printf("Draws:    %d\n", draws);         // show total draws
        printf("===================================\n\n");
        
        // ask if players want to play another round
        printf("Play again? (y/n): ");
        scanf(" %c", &playAgain); // read single character (space ignores whitespace)
        printf("\n");
        
    } while (playAgain == 'y' || playAgain == 'Y'); // repeat if user wants another game
    
    // program closing message
    printf("Thank you for playing!\n");
    // display final cumulative scores
    printf("Final Scores - X: %d, O: %d, Draws: %d\n", playerXScore, playerOScore, draws);
    
    // return 0 to indicate successful program termination
    return 0;
}

// ==================== function implementations ====================

// function: initializeBoard
// purpose: set up a fresh game board with all empty cells
// parameter: board - 2d array to be initialized (passed by reference)
// parameter: size - dimension of the board (size x size grid)
// return: void (modifies board in place)
// notes: must be called at the start of each new game
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    // outer loop iterates through all rows
    int i, j;
    for (i = 0; i < size; i++) {
        // inner loop iterates through all columns in current row
        for (j = 0; j < size; j++) {
            // set each cell to empty space character
            board[i][j] = ' ';
        }
    }
}

// function: printBoard
// purpose: display the current state of the game board to the console
// parameter: board - 2d array containing current board state (passed by reference)
// parameter: size - dimension of the board to determine grid size for display
// return: void (output only)
// notes: displays with grid lines and coordinates for easy user reference
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    printf("\n");
    
    // print column headers (numbers 0 to size-1)
    printf("   "); // indent to align with row numbers
    for (j = 0; j < size; j++) {
        printf(" %d  ", j); // each column number takes 4 spaces
    }
    printf("\n");
    
    // print top border of grid
    printf("  "); // indent for alignment
    for (j = 0; j < size; j++) {
        printf("+---"); // each cell has 3 dashes plus border
    }
    printf("+\n"); // closing border
    
    // print each row of the board
    for (i = 0; i < size; i++) {
        // print row number before the row
        printf("%d ", i);
        // print all cells in current row
        for (j = 0; j < size; j++) {
            printf("| %c ", board[i][j]); // each cell shows mark (x, o, or space)
        }
        printf("|\n"); // close row with right border
        
        // print horizontal separator after each row
        printf("  ");
        for (j = 0; j < size; j++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("\n");
}

// function: playerMove
// purpose: get move input from human player with input validation
// parameter: board - 2d array to place move on (passed by reference)
// parameter: size - board dimension for boundary checking
// parameter: player - 'x' or 'o' to place on board
// return: void (modifies board in place)
// notes: validates row/column bounds and cell availability before accepting move
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int row, col;           // coordinates for the move
    int validMove = 0;      // flag: 0 = invalid, 1 = valid and placed
    
    // loop until player provides valid move
    while (!validMove) {
        // get row coordinate from player
        printf("Enter row (0-%d): ", size - 1);
        scanf("%d", &row);
        // get column coordinate from player
        printf("Enter column (0-%d): ", size - 1);
        scanf("%d", &col);
        
        // validate that row and column are within board boundaries
        if (row < 0 || row >= size || col < 0 || col >= size) {
            // give feedback if coordinates are out of bounds
            printf("Invalid input! Row and column must be between 0 and %d.\n", size - 1);
        }
        // validate that selected cell is empty (not already occupied)
        else if (board[row][col] != ' ') {
            printf("Cell already occupied! Choose another cell.\n");
        }
        // if both validations pass, place move and exit loop
        else {
            board[row][col] = player; // mark board with player's symbol
            validMove = 1;            // set flag to exit loop
        }
    }
}

// function: aiMove
// purpose: generate ai move for week 1 (random placement only)
// parameter: board - 2d array to place ai move on (passed by reference)
// parameter: size - board dimension for random coordinate generation
// parameter: aiPlayer - 'x' or 'o' (typically 'o' as ai opponent to player x)
// return: void (modifies board in place)
// notes: week 1 implementation uses simple random move selection
// notes: week 2 will add strategic logic for winning and blocking moves
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer) {
    int row, col;
    
    // week 1 strategy: simple random move generation
    // keep generating random coordinates until an empty cell is found
    do {
        // generate random row: 0 to size-1
        row = rand() % size;
        // generate random column: 0 to size-1
        col = rand() % size;
        // repeat if selected cell is not empty
    } while (board[row][col] != ' ');
    
    // place ai mark on the randomly selected empty cell
    board[row][col] = aiPlayer;
    // display ai's move to the player
    printf("AI plays at row %d, column %d (random move)\n", row, col);
}

// function: checkWin
// purpose: determine if a player has achieved winning condition
// parameter: board - 2d array representing current board state (passed by reference)
// parameter: size - board dimension for checking all winning patterns
// parameter: player - character ('x' or 'o') of player to check win for
// return: int (1 if win detected, 0 if no win)
// notes: checks all possible winning combinations (rows, cols, diagonals)
// notes: must be called after each move to determine game status
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int i, j;
    int win; // flag for checking a particular line
    
    // check all rows for a complete line of player's marks
    for (i = 0; i < size; i++) {
        win = 1; // assume row is winning until proven otherwise
        for (j = 0; j < size; j++) {
            // if any cell in row doesn't match player, row is not a win
            if (board[i][j] != player) {
                win = 0;
                break; // exit inner loop early since row failed
            }
        }
        // if row passed all checks, player has won
        if (win) return 1;
    }
    
    // check all columns for a complete line of player's marks
    for (j = 0; j < size; j++) {
        win = 1; // assume column is winning until proven otherwise
        for (i = 0; i < size; i++) {
            // if any cell in column doesn't match player, column is not a win
            if (board[i][j] != player) {
                win = 0;
                break; // exit inner loop early since column failed
            }
        }
        // if column passed all checks, player has won
        if (win) return 1;
    }
    
    // check main diagonal (top-left to bottom-right)
    // main diagonal has coordinates (0,0), (1,1), (2,2), ..., (size-1, size-1)
    win = 1; // assume diagonal is winning until proven otherwise
    for (i = 0; i < size; i++) {
        // if any diagonal cell doesn't match player, diagonal is not a win
        if (board[i][i] != player) {
            win = 0;
            break; // exit loop early since diagonal failed
        }
    }
    // if diagonal passed all checks, player has won
    if (win) return 1;
    
    // check anti-diagonal (top-right to bottom-left)
    // anti-diagonal has coordinates (0, size-1), (1, size-2), ..., (size-1, 0)
    win = 1; // assume anti-diagonal is winning until proven otherwise
    for (i = 0; i < size; i++) {
        // if any anti-diagonal cell doesn't match player, it's not a win
        if (board[i][size - 1 - i] != player) {
            win = 0;
            break; // exit loop early since anti-diagonal failed
        }
    }
    // if anti-diagonal passed all checks, player has won
    if (win) return 1;
    
    // if no winning condition found in any direction, no win yet
    return 0;
}

// function: checkDraw
// purpose: determine if the game board is completely filled without a winner
// parameter: board - 2d array representing current board state (passed by reference)
// parameter: size - board dimension for checking all cells
// return: int (1 if draw condition detected, 0 if empty cells remain)
// notes: called after win check fails to determine game outcome
// notes: draw = no empty spaces remain AND no player has won
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    // scan entire board for any empty cells
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            // if an empty cell is found, game is not a draw
            if (board[i][j] == ' ') {
                return 0; // found an empty cell, game is not over
            }
        }
    }
    
    // if loop completes without finding empty cells, board is full
    return 1; // no empty cells found, board is completely filled
}

// function: updateScore
// purpose: increment the score counter for the winning player or draws
// parameter: winner - 'x' (player x won), 'o' (player o won), or 'd' (draw)
// return: void (modifies global score variables)
// notes: global variables (playerXScore, playerOScore, draws) are modified
// notes: scores persist across multiple game rounds within a session
void updateScore(char winner) {
    // check if player x won this round
    if (winner == 'X') {
        playerXScore++; // increment x's win counter
    }
    // check if player o won this round
    else if (winner == 'O') {
        playerOScore++; // increment o's win counter
    }
    // check if game ended in a draw
    else if (winner == 'D') {
        draws++; // increment draw counter
    }
}
