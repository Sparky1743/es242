/*
Team members: 
Birudugadda Srivibhav - 22110050
Bhoumik Patidar - 22110049
Praveen Rathod - 22110206
*/

/*
To run the code, use the command:
g++ -O2 -std=c++11 -Wall "-Wl,--stack=4084354560" up_it_up.cpp -o up_it_up.exe
*/
#include <cstdio>
#include <vector>
#include <array>
#include <algorithm>
#include <tuple>

#include "queue.hpp"
//0 for top, 
//1 for bottom, 
//2 for up, 
//3 for right, 
//4 for down, 
//5 for left, 
//6 for empty space
struct Board {
    int cells[3][3];
};

void PrintBoard(const Board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            printf("%3d", b.cells[r][c]);
        }
        printf("\n");
    }
}

void ReadBoard(Board& b) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            scanf("%d", &b.cells[r][c]);
        }
    }
}

void Swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

std::tuple<int, int> FindSpace(const Board& b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (b.cells[r][c] == 6) return {r, c};
    assert(0);
}

Board MoveUp(const Board& b) {
    const auto [r, c] = FindSpace(b);
    if (r == 0) return b;
    Board newBoard = b;
    switch (newBoard.cells[r - 1][c]) {
        case 0: newBoard.cells[r - 1][c] = 4; break;
        case 1: newBoard.cells[r - 1][c] = 2; break;
        case 2: newBoard.cells[r - 1][c] = 0; break;
        case 3: newBoard.cells[r - 1][c] = 3; break;
        case 4: newBoard.cells[r - 1][c] = 1; break;
        case 5: newBoard.cells[r - 1][c] = 5; break;
        default: assert(0);
    }
    Swap(newBoard.cells[r - 1][c], newBoard.cells[r][c]);

    return newBoard;
}

Board MoveDown(const Board& b) {
    const auto [r, c] = FindSpace(b);
    if (r == 2) return b;
    Board newBoard = b;
    switch (newBoard.cells[r + 1][c]) {
        case 0: newBoard.cells[r + 1][c] = 2; break;
        case 1: newBoard.cells[r + 1][c] = 4; break;
        case 2: newBoard.cells[r + 1][c] = 1; break;
        case 3: newBoard.cells[r + 1][c] = 3; break;
        case 4: newBoard.cells[r + 1][c] = 0; break;
        case 5: newBoard.cells[r + 1][c] = 5; break;
        default: assert(0);
    }
    Swap(newBoard.cells[r + 1][c], newBoard.cells[r][c]);

    return newBoard;
}

Board MoveLeft(const Board& b) {
    const auto [r, c] = FindSpace(b);
    if (c == 0) return b;
    Board newBoard = b;
    switch (newBoard.cells[r][c - 1]) {
        case 0: newBoard.cells[r][c - 1] = 3; break;
        case 1: newBoard.cells[r][c - 1] = 5; break;
        case 2: newBoard.cells[r][c - 1] = 2; break;
        case 3: newBoard.cells[r][c - 1] = 1; break;
        case 4: newBoard.cells[r][c - 1] = 4; break;
        case 5: newBoard.cells[r][c - 1] = 0; break;
        default: assert(0);
    }
    Swap(newBoard.cells[r][c - 1], newBoard.cells[r][c]);

    return newBoard;
}

Board MoveRight(const Board& b) {
    const auto [r, c] = FindSpace(b);
    if (c == 2) return b;
    Board newBoard = b;
    switch (newBoard.cells[r][c + 1]) {
        case 0: newBoard.cells[r][c + 1] = 5; break;
        case 1: newBoard.cells[r][c + 1] = 3; break;
        case 2: newBoard.cells[r][c + 1] = 2; break;
        case 3: newBoard.cells[r][c + 1] = 0; break;
        case 4: newBoard.cells[r][c + 1] = 4; break;
        case 5: newBoard.cells[r][c + 1] = 1; break;
        default: assert(0);
    }
    Swap(newBoard.cells[r][c + 1], newBoard.cells[r][c]);

    return newBoard;
}

bool IsSameBoard(const Board& a, const Board& b) {
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (a.cells[r][c] != b.cells[r][c]) return false;

    return true;
}

enum Move { LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4 };

int Ord(const Board& board) {
    int a = 0;
    int k = 1;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            a += (board.cells[row][col] * k);
            k = k * 7;
        }
    }
    return a;
}
/*
 * Return a shortest path from start to dest.
 */
#define MAX_STATES (40353608)

std::vector<int> SolvePuzzle(const Board& src, const Board& dest) {
    queue<Board, MAX_STATES> q;
    int visited[MAX_STATES];
    Board parent[MAX_STATES];

    enqueue(q, src);
    visited[Ord(src)] = LEFT;

    while (!is_empty(q)) {
        Board u = dequeue(q);
        if (IsSameBoard(u, dest)) {
            std::vector<int> moves;
            Board current = u;
            int order = Ord(current);
            while (!IsSameBoard(current, src)) {
                moves.push_back(visited[order]);
                current = parent[order];
                order = Ord(current);
            }
            std::reverse(moves.begin(), moves.end());
            return moves;
        }

        Board a = MoveUp(u);
        Board b = MoveDown(u);
        Board c = MoveLeft(u);
        Board d = MoveRight(u);

        int aOrder = Ord(a);
        int bOrder = Ord(b);
        int cOrder = Ord(c);
        int dOrder = Ord(d);

        if (!visited[aOrder]) {
            visited[aOrder] = UP;
            parent[aOrder] = u;
            enqueue(q, a);
        }
        if (!visited[bOrder]) {
            visited[bOrder] = DOWN;
            parent[bOrder] = u;
            enqueue(q, b);
        }
        if (!visited[cOrder]) {
            visited[cOrder] = LEFT;
            parent[cOrder] = u;
            enqueue(q, c);
        }
        if (!visited[dOrder]) {
            visited[dOrder] = RIGHT;
            parent[dOrder] = u;
            enqueue(q, d);
        }
    }
    printf("Not solvable\n");
    assert(0);
}

void PrintMoves(const std::vector<int>& moves) {
    for (auto move : moves) {
        switch (move) {
        case LEFT: printf("LEFT "); break;
        case RIGHT: printf("RIGHT "); break;
        case UP: printf("UP "); break;
        case DOWN: printf("DOWN "); break;
        }
    }
    printf("\n");
}

int main() {
    Board start, destination;
    // read_board(start);
    // read_board(destination);
    start.cells[0][0] = 1;
    start.cells[0][1] = 1;
    start.cells[0][2] = 1;
    start.cells[1][0] = 1;
    start.cells[1][1] = 6;
    start.cells[1][2] = 1;
    start.cells[2][0] = 1;
    start.cells[2][1] = 1;
    start.cells[2][2] = 1;
    
    destination.cells[0][0] = 0;
    destination.cells[0][1] = 0;
    destination.cells[0][2] = 0;
    destination.cells[1][0] = 0;
    destination.cells[1][1] = 6;
    destination.cells[1][2] = 0;
    destination.cells[2][0] = 0;
    destination.cells[2][1] = 0;
    destination.cells[2][2] = 0;

    auto moves = SolvePuzzle(start, destination);
    PrintMoves(moves);
    return 0;
}
