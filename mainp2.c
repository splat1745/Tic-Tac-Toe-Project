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
// ------------------------------------------------------------------
// These counters are declared globally to keep the examples simple for
// an introductory course: they are accessible from any function in this
// file without passing them as arguments. In production code, you might
// prefer to place them in a structure and pass a pointer.
//
// Rationale: using globals here simplifies the game loop and score
// display logic. For small educational programs this is acceptable, but
// be aware of the tradeoffs: global state can make unit testing and
// reasoning about function side effects harder.
int playerXScore = 0;
int playerOScore = 0;
int draws = 0;

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
    
    // Seed the random number generator used by the AI for fallback moves
    // ----------------------------------------------------------------
    // srand initializes the generator's state with a value that usually
    // changes (time(NULL)). Using a seed based on time ensures that the
    // sequence of random numbers (and therefore the AI's fallback moves)
    // is different each run. If reproducible play is required for testing
    // then this could be seeded with a fixed value instead.
    srand(time(NULL));
    
    printf("===================================\n");
    printf("  TIC-TAC-TOE GAME WITH AI (somewhat anyway)\n");
    printf("===================================\n\n");
    
    do {
        // Prompt for the board/grid size. The program supports boards
        // between 3x3 and MAX_SIZE x MAX_SIZE. A user-entered size is
        // validated to prevent out-of-bounds memory access on the board
        // array.
        do {
            printf("Enter grid size (3-10): ");
            scanf("%d", &size);
            if (size < 3 || size > MAX_SIZE) {
                printf("Invalid size! Please enter a value between 3 and 10.\n");
            }
        } while (size < 3 || size > MAX_SIZE);
        
        // Let the player choose the game mode. Two modes:
        // 1) Player vs Player: both players manually enter moves
        // 2) Player vs AI: a human player plays X, the program plays O
        // The loop prevents invalid choices. Separating the game mode
        // early keeps the game loop easier to follow because it can
        // branch on gameMode when taking turns.
        do {
            printf("\nSelect game mode:\n");
            printf("1. Player vs Player\n");
            printf("2. Player vs AI\n");
            printf("Enter your choice (1 or 2): ");
            scanf("%d", &gameMode);
            if (gameMode != 1 && gameMode != 2) {
                printf("Invalid choice! Please enter 1 or 2.\n");
            }
        } while (gameMode != 1 && gameMode != 2);
        
        // initialize the board
        initializeBoard(board, size);
        
        // Choose the starting player. This design sets X as the default
        // starting player so the UI remains predictable and tests are easy
        // to reproduce. It also keeps the game loop simple: flip currentPlayer
        // between 'X' and 'O' to switch turns.
        currentPlayer = 'X';
        gameOver = 0;
        
        printf("\n--- Game Start! ---\n");
        if (gameMode == 2) {
            printf("You are X, AI is O\n");
        }
        printf("\n");
        
        // main game loop
        while (!gameOver) {
            // Show the board on every iteration so players (and the AI)
            // can base decisions on the most recent state. Keeping display
            // separate from game logic makes the code cleaner and easier
            // to extend or reuse.
            printBoard(board, size);
            
            // Perform a move for the current player:
            // - If mode is PvP, both players are prompted for moves.
            // - If mode is PvAI, the human always plays X and the AI plays O.
            //   We explicitly check for the human player here to keep the
            //   control flow explicit and easy to follow.
            if (gameMode == 1 || currentPlayer == 'X') {
                // human player move
                printf("\nPlayer %c's turn:\n", currentPlayer);
                playerMove(board, size, currentPlayer);
            } else {
                // ai move
                printf("\nAI (O) is thinking...\n");
                aiMove(board, size, 'O');
            }
            
            // After each move we check for a win or a draw. These checks
            // are separated into functions to keep the main loop easy to
            // read and to promote code reuse (the AI can call the same
            // helpers to evaluate possible moves).
            if (checkWin(board, size, currentPlayer)) {
                printBoard(board, size);
                printf("\n*** Player %c wins! ***\n\n", currentPlayer);
                updateScore(currentPlayer);
                gameOver = 1;
            }
            // check for draw
            else if (checkDraw(board, size)) {
                printBoard(board, size);
                printf("\n*** It's a draw! ***\n\n");
                updateScore('D');
                gameOver = 1;
            }
            // switch players
            else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }
        
        // display current scores
        printf("===================================\n");
        printf("         SCORE BOARD\n");
        printf("===================================\n");
        printf("Player X: %d\n", playerXScore);
        printf("Player O: %d\n", playerOScore);
        printf("Draws:    %d\n", draws);
        printf("===================================\n\n");
        
        // Ask whether the user wants to play another game. The loop
        // reads a character and continues the outer loop if the answer
        // is 'y' or 'Y'. Using a char here is straightforward and
        // adequate for simple prompts; more robust input handling
        // would be required for a production-quality UI.
        printf("Play again? (y/n): ");
        scanf(" %c", &playAgain);
        printf("\n");
        
    } while (playAgain == 'y' || playAgain == 'Y');
    
    printf("Thank you for playing!\n");
    printf("Final Scores - X: %d, O: %d, Draws: %d\n", playerXScore, playerOScore, draws);
    
    return 0;
}

// initialize the board with empty spaces
void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;

    // We're using a fixed-size 2D array with dimensions MAX_SIZE x MAX_SIZE
    // even though the active board may be smaller (size x size). This
    // approach reduces dynamic memory complexity: there's no malloc/free
    // needed which keeps this example simpler for beginners. The loop
    // below ensures that only the active part (size x size) is initialized
    // so unused area (if any) is left alone but not used.
    //
    // The nested loops iterate row-by-row and set each cell to a space
    // character ' ' to indicate that the cell is empty. Using a space is a
    // convenient human-friendly default that prints nicely in the board.
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            // row-major indexing: board[row][col]
            board[i][j] = ' ';
        }
    }
}

// display the current state of the board
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    printf("\n");
    
    // Print column indices above the board to show coordinate system.
    // This helps users enter moves using row/col indices later on.
    // Aligning the numbers with printf(" %d  ") helps the grid remain
    // visually clean for square boards of various sizes.
    printf("   ");
    for (j = 0; j < size; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    // Print a top border using "+---" segments. The board printing uses
    // ASCII characters to draw a simple grid. This keeps the display
    // portable and readable in many terminal environments without relying
    // on external libraries.
    printf("  ");
    for (j = 0; j < size; j++) {
        printf("+---");
    }
    printf("+\n");
    
    // Iterate each row and print the row index followed by cell contents.
    // Cells are shown with vertical separators. This style is easy for
    // users to parse and for beginners to implement.
    for (i = 0; i < size; i++) {
        // Row label. Including the row number in the UI makes mapping
        // user inputs to array indices trivial (no off-by-one confusion).
        printf("%d ", i);
        for (j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
        
        // After printing row contents, print the separator line (+--- +---)
        // so the next row will appear under a clear dividing line.
        printf("  ");
        for (j = 0; j < size; j++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("\n");
}

// handle player move with input validation
void playerMove(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int row, col;
    int validMove = 0;
    
    // Loop until the player gives a valid move. This protects the
    // program from invalid indices (out-of-range) and from writing to
    // occupied cells which would otherwise corrupt the game state.
    // The loop uses a simple boolean flag validMove that flips to 1
    // only when input passes all validation checks.
    while (!validMove) {
        printf("Enter row (0-%d): ", size - 1);
        scanf("%d", &row);
        printf("Enter column (0-%d): ", size - 1);
        scanf("%d", &col);
        
        // Validate input for two conditions:
        // 1) Index bounds (row/col must be inside the size-by-size grid)
        // 2) The chosen cell must be empty
        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Invalid input! Row and column must be between 0 and %d.\n", size - 1);
        } else if (board[row][col] != ' ') {
            printf("Cell already occupied! Choose another cell.\n");
        } else {
            board[row][col] = player;
            validMove = 1;
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
    int i, j, k;
    int count, emptyRow, emptyCol;
    
    // First scan rows for a near-complete line which the player could
    // fill on the next turn. The algorithm counts how many of the cells
    // are occupied by 'player' and it records the location of exactly
    // one empty cell. If the count equals size-1 and there is one empty
    // cell, that empty cell is the winning move.
    for (i = 0; i < size; i++) {
        count = 0;
        emptyRow = -1;
        emptyCol = -1;
        for (j = 0; j < size; j++) {
            if (board[i][j] == player) {
                count++;
            } else if (board[i][j] == ' ') {
                emptyRow = i;
                emptyCol = j;
            }
        }
        // If the player already occupies size-1 cells in this row and a
        // single empty cell remains, then that empty cell is a winning
        // move: write the coordinates out through the pointers and tell
        // the caller that a winning opportunity exists.
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;
            *col = emptyCol;
            return 1;
        }
    }
    
    // Next check columns using the same logic. Notice that we switch
    // indexing: outer loop walks columns and inner loop walks rows so we
    // effectively mirror the row-checking logic but analyze vertical
    // lines. The variables emptyRow/emptyCol are reinitialized to ensure
    // we don't use stale values between column iterations.
    for (j = 0; j < size; j++) {
        count = 0;
        emptyRow = -1;
        emptyCol = -1;
        for (i = 0; i < size; i++) {
            if (board[i][j] == player) {
                count++;
            } else if (board[i][j] == ' ') {
                emptyRow = i;
                emptyCol = j;
            }
        }
        if (count == size - 1 && emptyRow != -1) {
            *row = emptyRow;
            *col = emptyCol;
            return 1;
        }
    }
    
    // Check the main diagonal (top-left to bottom-right). The diagonal
    // index is the same for row and column: (0,0), (1,1), ... (size-1,size-1).
    // The same size-1 + one empty cell logic is applied.
    count = 0;
    emptyRow = -1;
    emptyCol = -1;
    for (i = 0; i < size; i++) {
        if (board[i][i] == player) {
            count++;
        } else if (board[i][i] == ' ') {
            emptyRow = i;
            emptyCol = i;
        }
    }
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;
        *col = emptyCol;
        return 1;
    }
    
    // Check the anti-diagonal (top-right to bottom-left). The column
    // index for entry i is (size - 1 - i) while the row index is i.
    // This inspects the other diagonal and detects near-complete lines
    // the same way as above.
    count = 0;
    emptyRow = -1;
    emptyCol = -1;
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] == player) {
            count++;
        } else if (board[i][size - 1 - i] == ' ') {
            emptyRow = i;
            emptyCol = size - 1 - i;
        }
    }
    if (count == size - 1 && emptyRow != -1) {
        *row = emptyRow;
        *col = emptyCol;
        return 1;
    }
    
    return 0;
}

// enhanced ai move with strategic decision-making
void aiMove(char board[MAX_SIZE][MAX_SIZE], int size, char aiPlayer) {
    int row, col;
    char opponent = (aiPlayer == 'X') ? 'O' : 'X';
    
    // Strategy 1: If AI can win immediately on this move, take that
    // move. The canWin helper encodes the logic of scanning rows,
    // columns and diagonals for a near-win opportunity, and returns a
    // location if one exists. This is the highest priority because
    // winning ends the game.
    if (canWin(board, size, aiPlayer, &row, &col)) {
        board[row][col] = aiPlayer;
        printf("AI plays at row %d, column %d (Winning move!)\n", row, col);
        return;
    }
    
    // Strategy 2: If the opponent can win on their next move, block
    // them. That means detecting the opponent's near-win using canWin
    // and filling the required cell. Blocking takes second priority
    // because preventing immediate loss is critical.
    if (canWin(board, size, opponent, &row, &col)) {
        board[row][col] = aiPlayer;
        printf("AI plays at row %d, column %d (Blocking move!)\n", row, col);
        return;
    }
    
    // Strategy 3: For odd-sized boards, occupying the center is a strong
    // play. In many tic-tac-toe heuristics, the center increases the
    // number of lines you participate in (row, column, both diagonals
    // on square boards), so the center provides a higher chance of
    // creating future forks or potential wins. This is a rule of thumb
    // that holds in simple heuristics and is easy to implement.
    if (size % 2 == 1) {
        int center = size / 2;
        if (isCellEmpty(board, center, center)) {
            board[center][center] = aiPlayer;
            printf("AI plays at row %d, column %d (Center move!)\n", center, center);
            return;
        }
    }
    
    // Strategy 4: Corners are valuable on square boards because they
    // participate in fewer lines than center but more than edges in
    // some contexts (e.g., on a 3x3 board, corners can be part of two
    // potential winning lines). Taking a corner is often a good
    // fallback before resorting to random moves.
    int corners[4][2] = {{0, 0}, {0, size-1}, {size-1, 0}, {size-1, size-1}};
    for (int i = 0; i < 4; i++) {
        if (isCellEmpty(board, corners[i][0], corners[i][1])) {
            board[corners[i][0]][corners[i][1]] = aiPlayer;
            printf("AI plays at row %d, column %d (Corner move!)\n", corners[i][0], corners[i][1]);
            return;
        }
    }
    
    // Strategy 5 (fallback): pick a random valid empty cell. This keeps
    // the AI simple and ensures the function always returns a legal
    // move. Using rand() modulo the size produces a uniform index in
    // 0..size-1; the do/while loop repeats until an empty cell is found.
    // Note: in rare circumstances when almost the entire board is full
    // this will loop only a handful of times because it selects a
    // random location each iteration. For large boards and many filled
    // cells a more sophisticated selection (like collecting a list of
    // empty cells) would be more efficient, but this simple approach
    // is fine and easy to understand in this context.
    do {
        row = rand() % size;
        col = rand() % size;
    } while (!isCellEmpty(board, row, col));
    
    board[row][col] = aiPlayer;
    printf("AI plays at row %d, column %d\n", row, col);
}

// check if a player has won
int checkWin(char board[MAX_SIZE][MAX_SIZE], int size, char player) {
    int i, j;
    int win;
    
    // Check rows. For each row we optimistically assume win=1 (true) and
    // then scan the cells. If any cell does not match the player's symbol
    // we set win=0 and break out early. The early break reduces work when
    // a row fails quickly. Overall this is O(size * size) in the worst
    // case because we may need to inspect all cells in the board.
    for (i = 0; i < size; i++) {
        win = 1;
        for (j = 0; j < size; j++) {
            if (board[i][j] != player) {
                win = 0;
                break;
            }
        }
        // If the row contained only the player's marks the player has
        // a horizontal winning line and we return immediately. Early
        // returning saves unnecessary checking of other rows/columns.
        if (win) return 1;
    }
    
    // Check columns using the same approach. We check vertical
    // sequences by iterating columns in the outer loop and rows in the
    // inner loop. This consistent approach makes the function easy to
    // reason about and avoids duplicating logic.
    for (j = 0; j < size; j++) {
        win = 1;
        for (i = 0; i < size; i++) {
            if (board[i][j] != player) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    
    // main diagonal check: each diagonal index is (i,i)
    // The diagonal is only a winning line if ALL diagonal cells are
    // occupied by the same player. The 'win' flag is reused and
    // evaluated similarly to rows/columns.
    win = 1;
    for (i = 0; i < size; i++) {
        if (board[i][i] != player) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    // anti-diagonal check: indices are (i, size-1-i). The anti-diagonal
    // is the mirror image diagonal, and we check it the same way.
    win = 1;
    for (i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != player) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    return 0;
}

// check if the game is a draw
int checkDraw(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    
    // A simple draw check scans all cells. If any cell is still the
    // empty-space character ' ' there exists at least one legal move and
    // the game cannot be a draw. Note this function does not attempt to
    // detect forced-win sequences; it only determines if the board is
    // fully populated. This check is O(size*size) in time complexity.
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                return 0; // found an empty cell, game is not a draw
            }
        }
    }
    
    return 1; // no empty cells found, it's a draw
}

// update score based on winner
void updateScore(char winner) {
    // Simple scoreboard updater. We use 'X', 'O' and 'D' (draw) as
    // tokens to decide which global counter to increment. Using a
    // character code here keeps the interface small and makes calls
    // like updateScore('D') easy to read.
    if (winner == 'X') {
        playerXScore++;
    } else if (winner == 'O') {
        playerOScore++;
    } else if (winner == 'D') {
        draws++;
    }
}
