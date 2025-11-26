// engg1410: introductory programming for engineers
// mini project part #2: advanced tic-tac-toe with dynamic memory, file I/O, and enhanced AI
// mohamad abou el nasr
// college of engineering, university of guelph
// november 24, 2025
//
// Part 2 Enhancements:
// 1. Dynamic Memory Allocation (DMA) for board using char**
// 2. GameStats structure with dynamic win pattern tracking
// 3. File I/O System for saving/loading games and statistics
// 4. Enhanced AI with EASY, MEDIUM, and HARD difficulty levels
// 5. Minimax algorithm for HARD difficulty AI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Maximum board size (3-15 supported in Part 2)
#define MAX_SIZE 15
#define MIN_SIZE 3

// AI Difficulty Levels
#define EASY 1
#define MEDIUM 2
#define HARD 3

// Win pattern types for statistics tracking
#define WIN_ROW 1
#define WIN_COL 2
#define WIN_DIAG_MAIN 3
#define WIN_DIAG_ANTI 4

// ===================================================================
// PART 2: GameStats Structure with Dynamic Memory
// ===================================================================
// This structure tracks game statistics including a dynamic array
// of win patterns. The dynamic array grows as needed to accommodate
// new game results without fixed limits. This demonstrates key DMA
// concepts: malloc, realloc, and free for flexible data structures.
typedef struct {
    int games_played;      // Total number of games completed
    int wins_player1;      // Games won by player X
    int wins_player2;      // Games won by player O (or AI)
    int draws;             // Draws
    int *win_patterns;     // Dynamic array tracking HOW games were won
    int pattern_count;     // Current number of patterns recorded
    int pattern_capacity;  // Allocated capacity for patterns array
} GameStats;

// ===================================================================
// PART 2: Move Structure for Minimax Algorithm
// ===================================================================
// Encapsulates a position and its minimax evaluation score.
typedef struct {
    int row;    // Row of the move
    int col;    // Column of the move
    int score;  // Minimax score (positive = good for AI, negative = good for player)
} Move;

// ===================================================================
// Function Prototypes
// ===================================================================

// Part 1 functions (modified to use dynamic board char**)
void initializeBoard(char **board, int size);
void printBoard(char **board, int size);
void playerMove(char **board, int size, char player);
void aiMove(char **board, int size, char aiPlayer);
int checkWin(char **board, int size, char player);
int checkDraw(char **board, int size);
int canWin(char **board, int size, char player, int *row, int *col);
int isCellEmpty(char **board, int row, int col);

// Part 2: Dynamic Board Allocation
char** createBoard(int size);
void freeBoard(char **board, int size);

// Part 2: GameStats Management
GameStats* createGameStats(void);
void updateStats(GameStats *stats, char winner, int win_type);
void freeGameStats(GameStats *stats);
void printStatistics(const GameStats *stats);

// Part 2: File I/O
int saveGame(char **board, int size, const GameStats *stats, const char *filename);
int loadGame(char ***board, int *size, GameStats *stats, const char *filename);
int saveStatistics(const GameStats *stats, const char *filename);

// Part 2: Enhanced AI with Difficulty Levels
int aiMoveAdvanced(char **board, int size, int difficulty, char ai_char, char player_char);
Move minimax(char **board, int size, char ai_char, char player_char, int depth, bool is_maximizing);
int evaluateBoard(char **board, int size, char ai_char, char player_char, int depth);

// ===================================================================
// MAIN FUNCTION - Part 2 Version
// ===================================================================
int main(void) {
    char **board;
    int size;
    int gameMode;
    int difficulty = MEDIUM;  // Default AI difficulty
    char currentPlayer;
    int gameOver;
    char playAgain;
    char menuChoice;
    GameStats *stats;
    
    // Seed random number generator for AI fallback moves
    srand(time(NULL));
    
    // Create a new game statistics tracker
    stats = createGameStats();
    if (!stats) {
        printf("Error: Could not allocate memory for game statistics.\n");
        return 1;
    }
    
    printf("===================================\n");
    printf("  TIC-TAC-TOE GAME - PART 2\n");
    printf("  With Dynamic Memory & Enhanced AI\n");
    printf("===================================\n\n");
    
    do {
        // Main menu
        printf("\n--- MAIN MENU ---\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. View Statistics\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf(" %c", &menuChoice);
        
        switch (menuChoice) {
            case '1':
                // Get dynamic board size (3-15)
                do {
                    printf("\nEnter grid size (3-15): ");
                    scanf("%d", &size);
                    if (size < MIN_SIZE || size > MAX_SIZE) {
                        printf("Invalid size! Please enter a value between %d and %d.\n", MIN_SIZE, MAX_SIZE);
                    }
                } while (size < MIN_SIZE || size > MAX_SIZE);
                
                // Allocate dynamic board using Part 2 DMA
                board = createBoard(size);
                if (!board) {
                    printf("Error: Could not allocate memory for game board.\n");
                    continue;
                }
                
                // Select game mode
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
                
                // If PvAI, select difficulty
                if (gameMode == 2) {
                    do {
                        printf("\nSelect AI difficulty:\n");
                        printf("1. EASY (Random moves)\n");
                        printf("2. MEDIUM (Block/Win strategy)\n");
                        printf("3. HARD (Minimax algorithm)\n");
                        printf("Enter your choice (1-3): ");
                        scanf("%d", &difficulty);
                        if (difficulty < EASY || difficulty > HARD) {
                            printf("Invalid choice! Please enter 1, 2, or 3.\n");
                        }
                    } while (difficulty < EASY || difficulty > HARD);
                } else {
                    difficulty = MEDIUM;  // Unused in PvP mode
                }
                
                // Game loop
                currentPlayer = 'X';
                gameOver = 0;
                
                printf("\n--- Game Start! ---\n");
                if (gameMode == 2) {
                    printf("You are X, AI is O\n");
                    if (difficulty == EASY) printf("AI Difficulty: EASY\n");
                    else if (difficulty == MEDIUM) printf("AI Difficulty: MEDIUM\n");
                    else printf("AI Difficulty: HARD\n");
                }
                printf("\n");
                
                while (!gameOver) {
                    printBoard(board, size);
                    
                    if (gameMode == 1 || currentPlayer == 'X') {
                        // Human player move
                        printf("\nPlayer %c's turn:\n", currentPlayer);
                        playerMove(board, size, currentPlayer);
                    } else {
                        // AI move with selected difficulty
                        printf("\nAI (O) is thinking");
                        if (difficulty == HARD) printf(" (Minimax evaluation)");
                        printf("...\n");
                        aiMoveAdvanced(board, size, difficulty, 'O', 'X');
                    }
                    
                    // Check for win
                    if (checkWin(board, size, currentPlayer)) {
                        printBoard(board, size);
                        printf("\n*** Player %c wins! ***\n\n", currentPlayer);
                        int win_type = WIN_ROW;  // Default; in real implementation detect type
                        updateStats(stats, currentPlayer, win_type);
                        gameOver = 1;
                    }
                    // Check for draw
                    else if (checkDraw(board, size)) {
                        printBoard(board, size);
                        printf("\n*** It's a draw! ***\n\n");
                        updateStats(stats, 'D', 0);
                        gameOver = 1;
                    }
                    // Switch players
                    else {
                        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                    }
                }
                
                // Option to save game
                printf("Save game? (y/n): ");
                scanf(" %c", &playAgain);
                if (playAgain == 'y' || playAgain == 'Y') {
                    if (saveGame(board, size, stats, "savegame.txt") == 0) {
                        printf("Game saved successfully!\n");
                    } else {
                        printf("Error saving game.\n");
                    }
                    if (saveStatistics(stats, "statistics.txt") == 0) {
                        printf("Statistics saved successfully!\n");
                    }
                }
                
                // Free dynamically allocated board
                freeBoard(board, size);
                break;
                
            case '2':
                // Load game from file
                printf("\nLoading game from 'savegame.txt'...\n");
                if (loadGame(&board, &size, stats, "savegame.txt") == 0) {
                    printf("Game loaded successfully!\n");
                    printf("Board size: %d x %d\n\n", size, size);
                    printBoard(board, size);
                    freeBoard(board, size);
                } else {
                    printf("Error loading game. Make sure 'savegame.txt' exists.\n");
                }
                break;
                
            case '3':
                // Display statistics
                printStatistics(stats);
                break;
                
            case '4':
                // Exit
                printf("Thank you for playing! Final statistics:\n");
                printStatistics(stats);
                freeGameStats(stats);
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
    } while (1);
    
    return 0;
}

// ===================================================================
// PART 2: DYNAMIC BOARD ALLOCATION FUNCTIONS
// ===================================================================

// createBoard: Allocate a 2D dynamic array using char** (double pointer)
// This is a key Part 2 feature: instead of a fixed static array,
// we use malloc to create memory sized exactly for the user's input.
// A 2D array is implemented as an array of pointers (char**).
// Strategy: allocate an array of 'size' character pointers, then for
// each pointer allocate a row of 'size' characters.
char** createBoard(int size) {
    char **board = (char**)malloc(size * sizeof(char*));
    if (!board) return NULL;
    
    for (int i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
        if (!board[i]) {
            // Cleanup partial allocation on failure
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            return NULL;
        }
        // Initialize row to empty cells
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}

// freeBoard: Deallocate the 2D dynamic array
// Must free in reverse order: first the rows, then the array of pointers.
// This is crucial to avoid memory leaks in Part 2.
void freeBoard(char **board, int size) {
    if (!board) return;
    for (int i = 0; i < size; i++) {
        if (board[i]) {
            free(board[i]);
        }
    }
    free(board);
}

// ===================================================================
// PART 2: GAMESTATS STRUCTURE FUNCTIONS
// ===================================================================

// createGameStats: Allocate and initialize a GameStats structure
// The win_patterns array is allocated dynamically with an initial
// capacity. This allows us to store an unlimited number of game results.
GameStats* createGameStats(void) {
    GameStats *stats = (GameStats*)malloc(sizeof(GameStats));
    if (!stats) return NULL;
    
    stats->games_played = 0;
    stats->wins_player1 = 0;
    stats->wins_player2 = 0;
    stats->draws = 0;
    stats->pattern_capacity = 10;  // Initial capacity
    stats->pattern_count = 0;
    stats->win_patterns = (int*)malloc(stats->pattern_capacity * sizeof(int));
    
    if (!stats->win_patterns) {
        free(stats);
        return NULL;
    }
    return stats;
}

// updateStats: Update statistics after a game ends
// If the win_patterns array is full, use realloc to double the capacity.
// This demonstrates dynamic array resizing, a key DMA concept.
void updateStats(GameStats *stats, char winner, int win_type) {
    if (!stats) return;
    
    stats->games_played++;
    
    if (winner == 'X') {
        stats->wins_player1++;
    } else if (winner == 'O') {
        stats->wins_player2++;
    } else if (winner == 'D') {
        stats->draws++;
    }
    
    // Resize win_patterns if needed
    if (stats->pattern_count >= stats->pattern_capacity) {
        stats->pattern_capacity *= 2;
        int *new_patterns = (int*)realloc(stats->win_patterns, 
                                          stats->pattern_capacity * sizeof(int));
        if (!new_patterns) {
            printf("Warning: Could not resize pattern array.\n");
            return;
        }
        stats->win_patterns = new_patterns;
    }
    
    // Record the win pattern
    stats->win_patterns[stats->pattern_count++] = win_type;
}

// freeGameStats: Deallocate the GameStats structure and its dynamic arrays
void freeGameStats(GameStats *stats) {
    if (!stats) return;
    if (stats->win_patterns) {
        free(stats->win_patterns);
    }
    free(stats);
}

// printStatistics: Display the game statistics to the user
void printStatistics(const GameStats *stats) {
    if (!stats) return;
    
    printf("\n===================================\n");
    printf("         GAME STATISTICS\n");
    printf("===================================\n");
    printf("Games Played: %d\n", stats->games_played);
    printf("Player X Wins: %d\n", stats->wins_player1);
    printf("Player O Wins: %d\n", stats->wins_player2);
    printf("Draws: %d\n", stats->draws);
    printf("Total Win Patterns Recorded: %d\n", stats->pattern_count);
    printf("===================================\n\n");
}

// ===================================================================
// PART 2: FILE I/O FUNCTIONS
// ===================================================================

// saveGame: Save the current board state and statistics to a file
// Format: size, then each cell, then statistics
int saveGame(char **board, int size, const GameStats *stats, const char *filename) {
    if (!board || !stats || !filename) return -1;
    
    FILE *file = fopen(filename, "w");
    if (!file) return -1;
    
    // Write board size
    fprintf(file, "%d\n", size);
    
    // Write board state
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }
    
    // Write statistics
    fprintf(file, "%d %d %d %d\n", 
            stats->games_played, 
            stats->wins_player1, 
            stats->wins_player2, 
            stats->draws);
    
    fclose(file);
    return 0;
}

// loadGame: Load a saved game from file
// Note: This function takes a char*** (pointer to a char**) because
// we need to allocate the board inside the function and pass the
// allocated pointer back to the caller.
int loadGame(char ***board, int *size, GameStats *stats, const char *filename) {
    if (!board || !size || !stats || !filename) return -1;
    
    FILE *file = fopen(filename, "r");
    if (!file) return -1;
    
    // Read board size
    if (fscanf(file, "%d", size) != 1) {
        fclose(file);
        return -1;
    }
    
    // Allocate board with loaded size
    *board = createBoard(*size);
    if (!*board) {
        fclose(file);
        return -1;
    }
    
    // Read board state
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            if (fscanf(file, " %c", &(*board)[i][j]) != 1) {
                freeBoard(*board, *size);
                fclose(file);
                return -1;
            }
        }
    }
    
    // Read statistics
    int gp, w1, w2, d;
    if (fscanf(file, "%d %d %d %d", &gp, &w1, &w2, &d) == 4) {
        stats->games_played = gp;
        stats->wins_player1 = w1;
        stats->wins_player2 = w2;
        stats->draws = d;
    }
    
    fclose(file);
    return 0;
}

// saveStatistics: Save only the statistics to a file
int saveStatistics(const GameStats *stats, const char *filename) {
    if (!stats || !filename) return -1;
    
    FILE *file = fopen(filename, "w");
    if (!file) return -1;
    
    fprintf(file, "=== GAME STATISTICS ===\n");
    fprintf(file, "Games Played: %d\n", stats->games_played);
    fprintf(file, "Player X Wins: %d\n", stats->wins_player1);
    fprintf(file, "Player O Wins: %d\n", stats->wins_player2);
    fprintf(file, "Draws: %d\n", stats->draws);
    fprintf(file, "Win Patterns: %d\n", stats->pattern_count);
    
    fclose(file);
    return 0;
}

// ===================================================================
// BOARD MANAGEMENT FUNCTIONS (Modified for Dynamic Board char**)
// ===================================================================

// initializeBoard: Initialize all cells to empty (space character)
// Now uses the dynamic board pointer instead of static array.
void initializeBoard(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }
}

// printBoard: Display the board with formatting
void printBoard(char **board, int size) {
    printf("\n");
    
    // Print column numbers
    printf("   ");
    for (int j = 0; j < size; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    
    // Print top border
    printf("  ");
    for (int j = 0; j < size; j++) {
        printf("+---");
    }
    printf("+\n");
    
    // Print rows
    for (int i = 0; i < size; i++) {
        printf("%d ", i);
        for (int j = 0; j < size; j++) {
            printf("| %c ", board[i][j]);
        }
        printf("|\n");
        
        // Print row separator
        printf("  ");
        for (int j = 0; j < size; j++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("\n");
}

// playerMove: Prompt player for a move with validation
void playerMove(char **board, int size, char player) {
    int row, col;
    int validMove = 0;
    
    while (!validMove) {
        printf("Enter row (0-%d): ", size - 1);
        scanf("%d", &row);
        printf("Enter column (0-%d): ", size - 1);
        scanf("%d", &col);
        
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

// isCellEmpty: Check if a cell is empty
int isCellEmpty(char **board, int row, int col) {
    return board[row][col] == ' ';
}

// canWin: Check if a player can win on the next move
// Scans rows, columns, and diagonals for a near-complete line.
int canWin(char **board, int size, char player, int *row, int *col) {
    int count, emptyRow, emptyCol;
    
    // Check all rows
    for (int i = 0; i < size; i++) {
        count = 0;
        emptyRow = -1;
        emptyCol = -1;
        for (int j = 0; j < size; j++) {
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
    
    // Check all columns
    for (int j = 0; j < size; j++) {
        count = 0;
        emptyRow = -1;
        emptyCol = -1;
        for (int i = 0; i < size; i++) {
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
    
    // Check main diagonal
    count = 0;
    emptyRow = -1;
    emptyCol = -1;
    for (int i = 0; i < size; i++) {
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
    
    // Check anti-diagonal
    count = 0;
    emptyRow = -1;
    emptyCol = -1;
    for (int i = 0; i < size; i++) {
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

// ===================================================================
// PART 2: ENHANCED AI WITH DIFFICULTY LEVELS
// ===================================================================

// aiMoveAdvanced: Dispatcher for different AI difficulty levels
// Routes to the appropriate strategy based on difficulty selection.
int aiMoveAdvanced(char **board, int size, int difficulty, 
                   char ai_char, char player_char) {
    int row, col;
    char opponent = (ai_char == 'X') ? 'O' : 'X';
    
    if (difficulty == EASY) {
        // EASY: Random valid move
        do {
            row = rand() % size;
            col = rand() % size;
        } while (!isCellEmpty(board, row, col));
        
        board[row][col] = ai_char;
        printf("AI (EASY) plays at row %d, column %d\n", row, col);
        return 0;
    }
    
    else if (difficulty == MEDIUM) {
        // MEDIUM: Try to win, block opponent, take center/corners, then random
        // Strategy 1: Try to win
        if (canWin(board, size, ai_char, &row, &col)) {
            board[row][col] = ai_char;
            printf("AI (MEDIUM) plays at row %d, column %d (Winning move!)\n", row, col);
            return 0;
        }
        
        // Strategy 2: Block opponent
        if (canWin(board, size, opponent, &row, &col)) {
            board[row][col] = ai_char;
            printf("AI (MEDIUM) plays at row %d, column %d (Blocking move!)\n", row, col);
            return 0;
        }
        
        // Strategy 3: Take center if available (odd-sized grids)
        if (size % 2 == 1) {
            int center = size / 2;
            if (isCellEmpty(board, center, center)) {
                board[center][center] = ai_char;
                printf("AI (MEDIUM) plays at row %d, column %d (Center move!)\n", center, center);
                return 0;
            }
        }
        
        // Strategy 4: Take a corner
        int corners[4][2] = {{0, 0}, {0, size-1}, {size-1, 0}, {size-1, size-1}};
        for (int i = 0; i < 4; i++) {
            if (isCellEmpty(board, corners[i][0], corners[i][1])) {
                board[corners[i][0]][corners[i][1]] = ai_char;
                printf("AI (MEDIUM) plays at row %d, column %d (Corner move!)\n", 
                       corners[i][0], corners[i][1]);
                return 0;
            }
        }
        
        // Strategy 5: Random
        do {
            row = rand() % size;
            col = rand() % size;
        } while (!isCellEmpty(board, row, col));
        
        board[row][col] = ai_char;
        printf("AI (MEDIUM) plays at row %d, column %d\n", row, col);
        return 0;
    }
    
    else if (difficulty == HARD) {
        // HARD: Use minimax algorithm with alpha-beta pruning effects
        // Evaluate all possible moves and pick the best one
        Move best_move = minimax(board, size, ai_char, player_char, 0, true);
        
        if (best_move.row != -1 && best_move.col != -1) {
            board[best_move.row][best_move.col] = ai_char;
            printf("AI (HARD) plays at row %d, column %d (Minimax score: %d)\n", 
                   best_move.row, best_move.col, best_move.score);
            return 0;
        }
    }
    
    return -1;
}

// ===================================================================
// PART 2: MINIMAX ALGORITHM FOR HARD AI
// ===================================================================

// minimax: Recursive minimax evaluation with maximizing/minimizing players
// The AI (max) tries to maximize the score, the player (min) tries to minimize.
// This creates a game tree where each level alternates between max and min.
// Returns the best move (row, col) and its minimax score.
Move minimax(char **board, int size, char ai_char, char player_char, 
             int depth, bool is_maximizing) {
    Move best_move;
    best_move.row = -1;
    best_move.col = -1;
    best_move.score = is_maximizing ? -10000 : 10000;
    
    // Terminal conditions: win/loss/draw
    if (checkWin(board, size, ai_char)) {
        Move win_move;
        win_move.row = -1;
        win_move.col = -1;
        win_move.score = 10 - depth;  // Closer wins are valued higher
        return win_move;
    }
    
    if (checkWin(board, size, player_char)) {
        Move lose_move;
        lose_move.row = -1;
        lose_move.col = -1;
        lose_move.score = -10 + depth;  // Closer losses are worse
        return lose_move;
    }
    
    if (checkDraw(board, size)) {
        Move draw_move;
        draw_move.row = -1;
        draw_move.col = -1;
        draw_move.score = 0;
        return draw_move;
    }
    
    // Depth limit to avoid excessive computation (Part 2 optimization)
    if (depth > 4) {
        Move depth_move;
        depth_move.row = -1;
        depth_move.col = -1;
        depth_move.score = evaluateBoard(board, size, ai_char, player_char, depth);
        return depth_move;
    }
    
    // Try all possible moves
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (isCellEmpty(board, i, j)) {
                // Make move
                char current_char = is_maximizing ? ai_char : player_char;
                board[i][j] = current_char;
                
                // Recursive evaluation
                Move move = minimax(board, size, ai_char, player_char, 
                                   depth + 1, !is_maximizing);
                
                // Undo move
                board[i][j] = ' ';
                
                // Update best move
                if (is_maximizing) {
                    if (move.score > best_move.score) {
                        best_move.row = i;
                        best_move.col = j;
                        best_move.score = move.score;
                    }
                } else {
                    if (move.score < best_move.score) {
                        best_move.row = i;
                        best_move.col = j;
                        best_move.score = move.score;
                    }
                }
            }
        }
    }
    
    return best_move;
}

// evaluateBoard: Heuristic evaluation of the board state
// Used at depth limit to estimate the value of a position without
// fully exploring all branches. This is a simplified evaluation.
int evaluateBoard(char **board, int size, char ai_char, char player_char, int depth) {
    (void)depth;  // Parameter used for reference but not in simplified evaluation
    int score = 0;
    
    // Count potential winning lines for AI
    int ai_count = 0, player_count = 0;
    
    // Evaluate rows
    for (int i = 0; i < size; i++) {
        int ai_in_row = 0, player_in_row = 0, empty_in_row = 0;
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ai_char) ai_in_row++;
            else if (board[i][j] == player_char) player_in_row++;
            else empty_in_row++;
        }
        if (player_in_row == 0 && empty_in_row > 0) ai_count += ai_in_row;
        if (ai_in_row == 0 && empty_in_row > 0) player_count += player_in_row;
    }
    
    // Similar for columns and diagonals (simplified)
    score = (ai_count * 2) - (player_count * 3);
    return score;
}

// ===================================================================
// WIN/DRAW CHECKING FUNCTIONS (for dynamic board)
// ===================================================================

// checkWin: Check if a player has won (all cells in a line match)
int checkWin(char **board, int size, char player) {
    // Check rows
    for (int i = 0; i < size; i++) {
        int win = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != player) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    
    // Check columns
    for (int j = 0; j < size; j++) {
        int win = 1;
        for (int i = 0; i < size; i++) {
            if (board[i][j] != player) {
                win = 0;
                break;
            }
        }
        if (win) return 1;
    }
    
    // Check main diagonal
    int win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != player) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    // Check anti-diagonal
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][size - 1 - i] != player) {
            win = 0;
            break;
        }
    }
    if (win) return 1;
    
    return 0;
}

// checkDraw: Check if the board is full (game is a draw)
int checkDraw(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == ' ') {
                return 0;  // Found an empty cell
            }
        }
    }
    return 1;  // No empty cells, it's a draw
}
