// cs_moonlander.c
// Written by KIRAN KRISH (z5600259),
// on 12-10-2024
// TODO: Program Description

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Provided Constant Definitions
#define BOARD_LEN 10
#define BOARD_MAX_LEVELS 32
#define BASE_OXY_RATE 1.0
#define MAX_PORTALS 16
#define TITLE_STR_LEN 25
#define INVALID_INDEX -1
#define COMMAND_QUIT 'q'

// TODO: Your #defines go here
#define INITIAL_OXY_LEVEL 20.0
#define MAX_CHEESE 10

// Provided Enum Definitions
enum entity {
    EMPTY,
    CHEESE,
    ROCK,
    LANDER,
    HOLE_DOWN,
    HOLE_UP,
    PORTAL
};

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Provided Struct Definitions
struct tile {
    enum entity entity;
};

struct player {
    int row;
    int col;
    int cheese_held;
    double oxy_level;
    double oxy_capacity;
    double oxy_rate;
    int lander_row;
    int lander_col;
    int target_qty_cheese; // Added this field
};

// Provided Function Prototypes
void init_board(
    struct tile board[BOARD_LEN][BOARD_LEN]
);
void print_board(
    struct tile board[BOARD_LEN][BOARD_LEN],
    int player_row,
    int player_col,
    int cheese_held,
    int cheese_lander,
    double oxy_capacity,
    double oxy_level,
    double oxy_rate
);
void print_board_header(void);
void print_player_stats(
    int cheese_held,
    int cheese_lander,
    double oxy_capacity,
    double oxy_level,
    double oxy_rate
);

// TODO: Your function prototypes go here
int is_valid_coordinate(int row, int col);
void place_lander(struct tile board[BOARD_LEN][BOARD_LEN], struct player *p);
void place_cheese_and_rocks(struct tile board[BOARD_LEN][BOARD_LEN]);
int is_valid_tile(int row, int col, int rows, int cols, struct tile board[rows][cols]);
void place_large_rock(int start_row, int start_col, int end_row, int end_col, int rows, int cols, struct tile board[rows][cols]);
void setup_player(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN]);
void move_player(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], enum direction dir, int *cheese_lander);
void handle_turn(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], char command, int *cheese_lander);
void handle_moonquake(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], int *cheese_lander);
void refill_oxygen_if_near_lander(struct player *p);
int is_near_lander(struct player *p);
void drop_cheese_at_lander(struct player *p, int *cheese_lander);
void check_game_end_conditions(struct player *p, int cheese_lander); // Added this function prototype
void rotate_board_90_clockwise(struct tile board[BOARD_LEN][BOARD_LEN]);

int main(void) {
    struct tile board[BOARD_LEN][BOARD_LEN];
    struct player p;
    int cheese_lander = 0;
    
    init_board(board);
    place_lander(board, &p);
    place_cheese_and_rocks(board);
    setup_player(&p, board);

    char command;
    while (scanf(" %c", &command) != EOF) {
        if (command == COMMAND_QUIT) {
            break;
        }
        handle_turn(&p, board, command, &cheese_lander);
        check_game_end_conditions(&p, cheese_lander);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int is_valid_coordinate(int row, int col) {
    return row >= 0 && row < BOARD_LEN && col >= 0 && col < BOARD_LEN;
}

void place_lander(struct tile board[BOARD_LEN][BOARD_LEN], struct player *p) {
    int lander_row, lander_col;

    printf("Please enter the [row] [col] of the lander: ");
    scanf("%d %d", &lander_row, &lander_col);

    if (!is_valid_coordinate(lander_row, lander_col)) {
        lander_row = 4;
        lander_col = 4;
    }

    board[lander_row][lander_col].entity = LANDER;
    p->lander_row = lander_row;
    p->lander_col = lander_col;

    print_board(board, INVALID_INDEX, INVALID_INDEX, 0, 0, 0.0, 0.0, BASE_OXY_RATE);
}

void place_cheese_and_rocks(struct tile board[BOARD_LEN][BOARD_LEN]) {
    int row, col, start_row, start_col, end_row, end_col;
    char command;

    printf("Please enter cheese and rock locations:\n");

    while (scanf(" %c", &command) != EOF) {
        if (command == COMMAND_QUIT) {
            break;
        }
        if (command == 'c') {
            scanf("%d %d", &row, &col);
            if (!is_valid_coordinate(row, col) || board[row][col].entity != EMPTY) {
                printf("That is not a valid cheese placement!\n");
            } else {
                board[row][col].entity = CHEESE;
            }
        } else if (command == 'r') {
            scanf("%d %d", &row, &col);
            if (!is_valid_coordinate(row, col) || board[row][col].entity != EMPTY) {
                printf("That is not a valid rock placement!\n");
            } else {
                board[row][col].entity = ROCK;
            }
        } else if (command == 'R') {
            scanf("%d %d %d %d", &start_row, &start_col, &end_row, &end_col);
            place_large_rock(start_row, start_col, end_row, end_col, BOARD_LEN, BOARD_LEN, board);
        }
    }

    print_board(board, INVALID_INDEX, INVALID_INDEX, 0, 0, 0.0, 0.0, BASE_OXY_RATE);
}

int is_valid_tile(int row, int col, int rows, int cols, struct tile board[rows][cols]) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || board[row][col].entity != EMPTY) {
        return 0;
    }
    return 1;
}

void place_large_rock(int start_row, int start_col, int end_row, int end_col, int rows, int cols, struct tile board[rows][cols]) {
    if (start_row > end_row || start_col > end_col) {
        printf("That is not a valid rock placement!\n");
        return;
    }
    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            if (!is_valid_tile(i, j, rows, cols, board)) {
                printf("That is not a valid rock placement!\n");
                return;
            }
        }
    }
    for (int i = start_row; i <= end_row; i++) {
        for (int j = start_col; j <= end_col; j++) {
            board[i][j].entity = ROCK;
        }
    }
}

void setup_player(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN]) {
    int target_qty_cheese;
    printf("Please enter the target qty of cheese: ");
    while (scanf("%d", &target_qty_cheese) && target_qty_cheese < 0) {
        printf("The target qty of cheese must be >= 0!\n");
    }

    p->target_qty_cheese = target_qty_cheese; // Store target cheese

    double oxy_capacity;
    printf("Please enter the player's oxygen tank capacity: ");
    while (scanf("%lf", &oxy_capacity) && oxy_capacity < 0) {
        printf("The oxygen tank capacity must be >= 0!\n");
    }

    int player_row, player_col;
    printf("Please enter the [row] [col] of the player: ");
    while (scanf("%d %d", &player_row, &player_col) &&
           (!is_valid_coordinate(player_row, player_col) || board[player_row][player_col].entity != EMPTY)) {
        printf("That is not a valid player placement!\n");
    }

    p->row = player_row;
    p->col = player_col;
    p->cheese_held = 0;
    p->oxy_capacity = oxy_capacity;
    p->oxy_level = oxy_capacity;
    p->oxy_rate = BASE_OXY_RATE; // Initialize oxygen rate

    printf("<->        STARTING MOONLANDER        <->\n");
    print_board(board, p->row, p->col, p->cheese_held, 0, p->oxy_capacity, p->oxy_level, p->oxy_rate);
}

void move_player(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], enum direction dir, int *cheese_lander) {
    int new_row = p->row;
    int new_col = p->col;

    if (dir == UP) new_row--;
    else if (dir == DOWN) new_row++;
    else if (dir == LEFT) new_col--;
    else if (dir == RIGHT) new_col++;

    if (!is_valid_coordinate(new_row, new_col) || board[new_row][new_col].entity == ROCK || board[new_row][new_col].entity == LANDER) {
        print_board(board, p->row, p->col, p->cheese_held, *cheese_lander, p->oxy_capacity, p->oxy_level, p->oxy_rate);
        return;
    }

    p->row = new_row;
    p->col = new_col;

    if (board[new_row][new_col].entity == CHEESE) {
        p->cheese_held++;
        board[new_row][new_col].entity = EMPTY;
    }

    p->oxy_level -= p->oxy_rate;

    // Refill oxygen and repair suit if near the lander
    if (is_near_lander(p)) {
        drop_cheese_at_lander(p, cheese_lander);
        refill_oxygen_if_near_lander(p);
    }
    print_board(board, p->row, p->col, p->cheese_held, *cheese_lander, p->oxy_capacity, p->oxy_level, BASE_OXY_RATE);
}

void handle_turn(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], char command, int *cheese_lander) {
    if (command == 'w') move_player(p, board, UP, cheese_lander);
    else if (command == 'a') move_player(p, board, LEFT, cheese_lander);
    else if (command == 's') move_player(p, board, DOWN, cheese_lander);
    else if (command == 'd') move_player(p, board, RIGHT, cheese_lander);
    else if (command == 'm') handle_moonquake(p, board, cheese_lander); // Added moonquake command
    else printf("Command not recognised!\n");
}

void handle_moonquake(struct player *p, struct tile board[BOARD_LEN][BOARD_LEN], int *cheese_lander) {
    // Rotate the board
    rotate_board_90_clockwise(board);

    // Adjust player's position
    int new_row = p->col;
    int new_col = BOARD_LEN - 1 - p->row;
    p->row = new_row;
    p->col = new_col;

    // Decrement oxygen level by 20% of the tank's maximum capacity
    p->oxy_level -= p->oxy_capacity * 0.2;

    // Ensure oxygen level does not drop below zero
    if (p->oxy_level < 0) {
        p->oxy_level = 0;
    }

    // Increase oxygen consumption rate by 20%
    p->oxy_rate *= 1.2;

    // Refill oxygen and repair suit if near the lander
    refill_oxygen_if_near_lander(p);

    // Print the board after all effects
    print_board(board, p->row, p->col, p->cheese_held, *cheese_lander, p->oxy_capacity, p->oxy_level, p->oxy_rate);
}

void refill_oxygen_if_near_lander(struct player *p) {
    if (abs(p->row - p->lander_row) <= 1 && abs(p->col - p->lander_col) <= 1) {
        p->oxy_level = p->oxy_capacity;
        p->oxy_rate = BASE_OXY_RATE; // Reset oxygen rate when near the lander
        p->cheese_held = 0; // Drop off all held cheese
    }
}

int is_near_lander(struct player *p) {
    return abs(p->row - p->lander_row) <= 1 && abs(p->col - p->lander_col) <= 1;
}

void drop_cheese_at_lander(struct player *p, int *cheese_lander) {
    *cheese_lander += p->cheese_held;
    p->cheese_held = 0;
}

void check_game_end_conditions(struct player *p, int cheese_lander) {
    // Check if the player has won
    if (cheese_lander >= p->target_qty_cheese) {
        printf("Congratulations, you won!\n");
        exit(0);
    }

    // Check if the player has lost
    if (p->oxy_level <= 0) {
        printf("Sorry, you ran out of oxygen and lost!\n");
        exit(0);
    }
}

void rotate_board_90_clockwise(struct tile board[BOARD_LEN][BOARD_LEN]) {
    struct tile temp[BOARD_LEN][BOARD_LEN];

    for (int row = 0; row < BOARD_LEN; row++) {
        for (int col = 0; col < BOARD_LEN; col++) {
            temp[col][BOARD_LEN - 1 - row] = board[row][col];
        }
    }

    for (int row = 0; row < BOARD_LEN; row++) {
        for (int col = 0; col < BOARD_LEN; col++) {
            board[row][col] = temp[row][col];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Initialises the board to EMPTY for each tile
void init_board(struct tile board[BOARD_LEN][BOARD_LEN]) {
    for (int row = 0; row < BOARD_LEN; row++) {
        for (int col = 0; col < BOARD_LEN; col++) {
            board[row][col].entity = EMPTY;
        }
    }
    return;
}

// Prints a line commonly used when printing the board, line length scales with
// the BOARD_LEN constant
void print_board_line(void) {
    printf("+");
    for (int col = 0; col < BOARD_LEN; col++) {
        printf("---+");
    }
    printf("\n");
    return;
}

// Prints the header of the cs_moonlander board
void print_board_header(void) {
    char title_str[TITLE_STR_LEN + 1] = "C S   M O O N L A N D E R";

    printf("|");
    for (int i = (4 * BOARD_LEN - TITLE_STR_LEN) / 2; i > 0; i--) {
        printf(" ");
    }

    printf("%s", title_str);

    for (int i = (4 * BOARD_LEN - TITLE_STR_LEN) / 2; i > 0; i--) {
        printf(" ");
    }
    printf("|\n");

    return;
}

// Prints the player stats when requested by the user
void print_player_stats(
    int cheese_held,
    int cheese_lander,
    double oxy_capacity,
    double oxy_level,
    double oxy_rate
) {
    printf("Player Cheese: %d     Lander Cheese: %d\n",
            cheese_held, cheese_lander);
    printf("Oxy: %.2lf / %.2lf  @  %.6lf / move\n", oxy_level,
            oxy_capacity, oxy_rate);
    return;
}

// Prints the cs_moonlander board
void print_board(
    struct tile board[BOARD_LEN][BOARD_LEN],
    int player_row,
    int player_col,
    int cheese_held,
    int cheese_lander,
    double oxy_capacity,
    double oxy_level,
    double oxy_rate) {

    print_board_line();
    print_board_header();
    print_board_line();
    for (int row = 0; row < BOARD_LEN; row++) {
        for (int col = 0; col < BOARD_LEN; col++) {
            printf("|");
            if (row == player_row && col == player_col) {
                printf("0.0");
            } else if (board[row][col].entity == ROCK) {
                printf("^^^");
            } else if (board[row][col].entity == LANDER) {
                printf("/|\\");
            } else if (board[row][col].entity == CHEESE) {
                printf("<(]");
            } else if (board[row][col].entity == HOLE_UP) {
                printf("/O\\");
            } else if (board[row][col].entity == HOLE_DOWN) {
                printf("\\O/");
            } else if (board[row][col].entity == PORTAL) {
                printf("~~~");
            } else {
                printf("   ");
            }
        }
        printf("|\n");
        print_board_line();
    }
    print_player_stats(cheese_held, cheese_lander, oxy_capacity, oxy_level,
            oxy_rate);
    print_board_line();
    return;
}
