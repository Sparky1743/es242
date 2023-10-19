/*
Team members: 
Birudugadda Srivibhav - 22110050
Bhoumik Patidar - 22110049
Praveen Rathod - 22110206
*/
#include <stdio.h>
#include <assert.h>

/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
enum {
    EMPTY = 0,
    RED = 1,
    BLUE = 2,
};

typedef char board_t[4][5];
typedef char player_t;

void init_board(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            board[row][col] = EMPTY;
        }
    }
}

player_t other_player(player_t player)
{
    switch (player) {
        case RED: return BLUE;
        case BLUE: return RED;
        default: assert(0);
    }
}

int has_won(board_t board, player_t player)
{
    // Horizontal
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row][col + 1] == player &&
                board[row][col + 2] == player &&
                board[row][col + 3] == player) {
                return 1;
            }
        }
    }

    // Vertical
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 1; row++) {
            if (board[row][col] == player &&
                board[row + 1][col] == player &&
                board[row + 2][col] == player &&
                board[row + 3][col] == player) {
                return 1;
            }
        }
    }

    // Diagonal1
    for (int row = 0; row < 1; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player &&
                board[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }

    // Diagonal2
    for (int row = 3; row >= 2; row--) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row - 1][col + 1] == player &&
                board[row - 2][col + 2] == player &&
                board[row - 3][col + 3] == player) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            if (board[row][col] == EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

int column_full(board_t board, int col)
{
    return board[0][col] != EMPTY;
}

int drop_piece(board_t board, int col, player_t player)
{
    for (int row = 3; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = player;
            return row; 
        }
    }
    return -1; 
}

typedef struct {
    int col;
    int score;
} move_t;

move_t best_move(board_t board, player_t player, int depth) {
    move_t response;
    move_t candidate = {-1, -1};
    int no_candidate = 1;

    if (depth == 0 || is_full(board)) {
        candidate.score = 0;
        return candidate;
    }

    for (int col = 0; col < 5; ++col) {
        if (!column_full(board, col)) {
            int row = drop_piece(board, col, player);
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = EMPTY;
                return (move_t){col, 1};
            }
            board[row][col] = EMPTY;
        }
    }
    
    for (int col = 0; col < 5; ++col) {
        if (!column_full(board, col)) {
            int row = drop_piece(board, col, player);
            board[row][col] = player;
            if (is_full(board)) {
                board[row][col] = EMPTY;
                return (move_t){col, 0};
            }
            response = best_move(board, other_player(player), depth - 1);
            board[row][col] = EMPTY;

            if (response.score == -1) {
                return (move_t){col, 1};
            } else if (response.score == 0) {
                candidate = (move_t){col, 0};
                no_candidate = 0;
            } else{
                if (no_candidate) {
                    candidate = (move_t){col, -1};
                    no_candidate = 0;
                }
            }
        }
    }

    return candidate;
}


void print_board(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            switch (board[row][col]) {
                case EMPTY: printf("0  "); break;
                case RED: printf("R  "); break;
                case BLUE: printf("B  "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    int move, col;
    board_t board;
    move_t response;
    int x;
    printf("You are red colour, Do you choose to be player 1 or 2: ");
    scanf("%d", &x);
    player_t current;

    if (x == 1) {
        current = RED;
    } else if (x == 2) {
        current = BLUE;
    } else {
        printf("\nInvalid choice");
        return 1; // Exit the program due to invalid choice
    }


    init_board(board);

    while (1) {
        if (x != 1 && x != 2) {
            printf("\nInvalid choice");
            break;
        }
        print_board(board);
        printf("\n\n");

        if (current == RED) {
            printf("0  1  2  3  4\n");
            printf("\nEnter your move: ");
            scanf("%d", &move);
            col = move;

            if (col >= 0 && col < 5 && !column_full(board, col)) {
                drop_piece(board, col, current);
            } else {
                printf("Invalid move. Try again.\n");
                continue;
            }
        } else {
            response = best_move(board, current, 11);
            col = response.col;

            if (col >= 0 && col < 5 && !column_full(board, col)) {
                int row = drop_piece(board, col, current);
            }
        }

        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", (current == RED) ? 'R' : 'B');
            break;
        }else if (is_full(board)) {
            printf("It's a draw!\n");
            break;
        }

        current = other_player(current);
    }
    return 0;
}