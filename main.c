#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>

#define BLANK 0



unsigned int board[4][4];
unsigned int score = 0;
bool won = false;

typedef enum {
    UP, DOWN, LEFT, RIGHT, END
} key;

void INIT_2048() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(NULL));
}

void DONE_2048() {
    endwin();
    exit(0);
}

void display_board() {
    clear();
    move(0, 0);
    printw("== 2048 =============================\n");
    printw("====================== score: %7i\n", score);
    printw("\n");
    printw("+--------+--------+--------+--------+\n");
    for (int row = 0; row < 4; row++) {
        printw("|        |        |        |        |\n");
        printw("|");
        for (int col = 0; col < 4; col++) {
            if (board[row][col]) {
                printw(" %5i  |", board[row][col]);
            } else {
                printw("        |");
            }
        }
        printw("\n");
        printw("|        |        |        |        |\n");
        printw("+--------+--------+--------+--------+\n");
    }
    printw("\n");
    refresh();
}

void add_two() {
    int empty_tiles[16];
    int empty_count = 0;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (board[row][col] == BLANK) {
                empty_tiles[empty_count] = row * 4 + col;
                empty_count++;
            }
        }
    }

    if (empty_count > 0) {
        int index = rand() % empty_count;
        int row = empty_tiles[index] / 4;
        int col = empty_tiles[index] % 4;
        board[row][col] = 2;
    }
}

void reset_board() {
    score = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            board[i][j] = BLANK;
        }
    }
    add_two();
    add_two();
}

bool game_lost() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int tile_value = board[i][j];
            if ((tile_value == BLANK) || 
                (i != 3 && board[i + 1][j] == tile_value) ||
                (j != 3 && board[i][j + 1] == tile_value) ||
                (i != 0 && board[i - 1][j] == tile_value) ||
                (j != 0 && board[i][j - 1] == tile_value)) {
                return false;
            }
        }
    }
    return true;
}

enum MoveType {
    NOMOVE,
    MOVE,
    MERGE,
    WIN
};

enum MoveType move_tile(int row, int column, key direction, bool dont_merge) {
    int tile_value = board[row][column];
    if (tile_value == BLANK) {
        return NOMOVE;
    }

    int next_row = row;
    int next_column = column;
    switch (direction) {
        case RIGHT:
            next_column = column + 1;
            break;
        case LEFT:
            next_column = column - 1;
            break;
        case UP:
            next_row = row - 1;
            break;
        case DOWN:
            next_row = row + 1;
            break;
        default:
            return NOMOVE;
    }

    if (next_row < 0 || next_row >= 4 || next_column < 0 || next_column >= 4) {
        return NOMOVE;
    }

    int next_value = board[next_row][next_column];
    if (next_value == BLANK) {
        board[row][column] = BLANK;
        board[next_row][next_column] = tile_value;
        return MOVE;
    }
    else if (next_value == tile_value && !dont_merge) {
        board[row][column] = BLANK;
        int new_value = tile_value << 1;
        if (new_value == 2048) {
            won = true;
            return WIN;
        }
        board[next_row][next_column] = new_value;
        score += new_value;
        return MERGE;
    } else {
        return NOMOVE;
    }
}

bool move_board(key direction) {
    int row_start = 0;
    int row_increment = 1;
    int row_end = 4;
    int column_start = 0;
    int column_increment = 1;
    int column_end = 4;
    switch (direction) {
        case LEFT: 
            column_start = 1;
            break;
        case RIGHT: 
            column_start = 2;
            column_increment = -1;
            column_end = -1;
            break;
        case UP: 
            row_start = 1;
            break;
        case DOWN: 
            row_start = 2;
            row_increment = -1;
            row_end = -1;
            break;
    }

    bool merged[16] = {false};
    int tile_index;
    bool moved = false;
    enum MoveType move_type;
    for (int s = 0; s < 3; s++) {
        for (int i = row_start; i != row_end; i += row_increment) {
            for (int j = column_start; j != column_end; j += column_increment) {
                tile_index = (i << 2) | j;
                move_type = move_tile(i, j, direction, merged[tile_index]);
                if (move_type == WIN) {
                    return true;
                }
                else if (move_type == MERGE) {
                    merged[tile_index] = true;
                    int next_row = i + ((direction == UP || direction == DOWN) ? row_increment * -1 : 0);
                    int next_column = j + ((direction == RIGHT || direction == LEFT) ? column_increment * -1 : 0);
                    int next_tile_index = (next_row << 2) | next_column;
                    merged[next_tile_index] = true;
                }
                else if (move_type != NOMOVE) {
                    moved = true;
                }
            }
        }
        
        display_board();
    }
    return moved;
}

key get_key() {
    int ch;
    while (1) {
        ch = getch();
        switch (ch) {
            case KEY_UP: return UP;
            case KEY_DOWN: return DOWN;
            case KEY_LEFT: return LEFT;
            case KEY_RIGHT: return RIGHT;
            case 'q': return END;
        }
    }
}

void game_over() {
    display_board();
    printw("============= GAME OVER =============\n");
    printw("=========== (press a key) ===========\n");
    refresh();
    getch();
    
}

int main() {
    INIT_2048();
    reset_board();
    display_board();
    key pressed_key;
    while (1) {
        pressed_key = get_key();
        if (pressed_key == END) {
            DONE_2048();
        }
        if (move_board(pressed_key)) {
            add_two();
        }
        display_board();
        if (won) {
            printw("============= YOU WIN =============\n");
            printw("=========== (press a key) ===========\n");
            refresh();
            getch();
            reset_board();
            won = false;
        }
        if (game_lost()) {
            game_over();
            reset_board();
        }
    }
    return 0;
}
