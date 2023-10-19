/*
Team members: 
Birudugadda Srivibhav - 22110050
Bhoumik Patidar - 22110049
Praveen Rathod - 22110206
*/
#include <stdio.h>
#include <assert.h>
//With lousy implementation of player 2
enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

typedef char board_t[15];
typedef char player_t;

void init_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        board[i] = NO;
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
    int a[4] = {4, 7, 9, 10};
    int b[3] = {3, 5, 6};
    int c[2] = {2, 3};
    int d = 1;
    
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            if (board[i] == other_player(player) &&
                board[j] == other_player(player) &&
                board[j + a[i]] == other_player(player)) {
                return 1;
            }
        }
    }

    for (int i = 5; i < 8; i++) {
        for (int j = i + 1; j <= 8; j++) {
            if (board[i] == other_player(player) &&
                board[j] == other_player(player) &&
                board[j + b[i - 5]] == other_player(player)) {
                return 1;
            }
        }
    }

    for (int i = 9; i < 11; i++) {
        for (int j = i + 1; j <= 11; j++) {
            if (board[i] == other_player(player) &&
                board[j] == other_player(player) &&
                board[j + c[i - 9]] == other_player(player)) {
                return 1;
            }
        }
    }

    for (int i = 12; i < 13; i++) {
        for (int j = i + 1; j <= 13; j++) {
            if (board[i] == other_player(player) &&
                board[j] == other_player(player) &&
                board[j + d] == other_player(player)) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board)  //---- > no need as the game never ties....
{
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    int line;
    int score;
} move_t;

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t candidate;
    int no_candidate = 1;

    //Base case
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            if (has_won(board, player)) {
                board[i] = NO;
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }

    //Predictions
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            response = best_move(board, other_player(player));
            board[i] = NO;
            if (is_full(board)) {
                board[i] = NO;
                return (move_t) {i, 0};
            }
            if (response.score == -1) {
                return (move_t){i, 1};
            } else if (response.score == 0) {
                candidate = (move_t){i, 0};
                no_candidate = 0;
            } else {
                if (no_candidate) {
                    candidate = (move_t){i, -1};
                    no_candidate = 0;
                }
            }
        }
    }

    return candidate;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
            case NO: printf("0  "); break;
            case RED: printf("R  "); break;
            case BLUE: printf("B  "); break;
        }
    }
    printf("\n");
}

int main()
{
    int move, i;
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
        print_board(board);
        printf("\n\n");

        if (current == RED) {
            printf("0  1  2  3  4  5  6  7  8  9 10 11 12 13 14\n");
            printf("\nEnter your move: ");
            scanf("%d", &move);
            i = move;
            if (i >= 0 && i < 15 && board[i] == NO) {
                board[i] = current;
            } 
            else {
                printf("Invalid move. Try again.\n");
                continue;
            }
        } else {
            response = best_move(board, current);
            board[response.line] = current;
        }

        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", (current == RED) ? 'R' : 'B');
            break;
        }

        current = other_player(current);
    }

    return 0;
}