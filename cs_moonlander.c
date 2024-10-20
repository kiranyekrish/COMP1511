// cs_moonlander.c
// Written by YOUR-NAME (YOUR-ZID),
// on TODAYS-DATE
// TODO: Program Description

#include <stdio.h>

// Provided Constant Definitions
#define BOARD_LEN 10
#define BOARD_MAX_LEVELS 32
#define BASE_OXY_RATE 1.0
#define MAX_PORTALS 16
#define TITLE_STR_LEN 25
#define INVALID_INDEX -1
#define COMMAND_QUIT 'q'

// TODO: Your #defines go here

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

// TODO: Your enums go here

// Provided Struct Definitions
struct tile {
    enum entity entity;
};

// TODO: Your structs go here

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

int main(void) {
    // TODO: add your code (and remove this todo)

    // prints the board with no player
    // you may decide to delete or move this later...
    struct tile board[BOARD_LEN][BOARD_LEN];
    init_board(board);
    print_board(board, INVALID_INDEX, INVALID_INDEX, 0, 0, 0.0, 0.0,
            BASE_OXY_RATE);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: Your function implementations go here

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
