// Assignment 2 24T3 COMP1511: CS Dungeon
// cs_dungeon.c
//
// This program was written by Kiran Krish, z5600259
// on 12-11-2024
//
// Version 1.0.0: Assignment released.
//
// <DESCRIPTION OF YOUR PROGRAM HERE>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_dungeon.h"

// Add any extra #includes your code needs here.

////////////////////////////////////////////////////////////////////////////////
//                                 Constants                                  //
////////////////////////////////////////////////////////////////////////////////

// Add your own #defines here:

// Your enums go here:


////////////////////////////////////////////////////////////////////////////////
//                            Struct Definitions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided structs:

// Stores information about a dungeon
struct dungeon {
    // name of the dungeon
    char name[MAX_STR_LEN];

    // what type of monster it contains (enum described in cs_dungeon.h)
    enum monster_type monster;

    // the amount of monsters it contains
    int num_monsters;

    // a pointer to the boss monster (if it contains one) or NULL
    struct boss *boss;

    // a pointer to the first item it contains or NULL
    struct item *items;

    // an int indicating if the player is inside the dungeon
    //      1: The player is inside the dungeon
    //      0: The player is not inside the dungeon
    int contains_player;

    // a pointer to the next dungeon in the map or NULL
    struct dungeon *next;
};

// Stores information about an item
struct item {
    // what type of item (enum described in cs_dungeon.h)
    enum item_type type;

    // how many points the item is worth
    int points;

    // a pointer to the next item in the list or NULL
    struct item *next;
};

// Stores information about the player who is exploring the dungeon map
struct player {
    // name of the player
    char name[MAX_STR_LEN];

    // what type of class they are playing (Figher or Wizard)
    char class_type[MAX_STR_LEN];

    // how many health points they have
    int health_points;

    // how much shield power they have
    int shield_power;

    // how much damage they have
    int damage;

    // what their magic modifier is when they make magical attacks
    double magic_modifier;

    // a pointer to the first item in their inventory or NULL
    struct item *inventory;

    // the number of points they have collected over the course of the game
    int points;
};


// Stores information about the boss-level monster initially placed in the last
// dungeon in the map
struct boss {
    // how many health points they have
    int health_points;

    // how much damage they have
    int damage;

    // how many points they are worth when defeated
    int points;

    // the item type the player is required to have to fight them
    enum item_type required_item;
};

// Add any other structs you define here.

////////////////////////////////////////////////////////////////////////////////
//                           Additional Prototypes                            //
////////////////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.

// Stage 1
struct dungeon *create_dungeon(char *name, enum monster_type monster,
                               int num_monsters, int contains_player);

struct boss *create_boss(int health_points, int damage, int points,
                         enum item_type required_item);

// Stage 2

// Stage 3

// Stage 4

// You need to implement the following functions.
// You can find descriptions of what each function
// should do in dungeon.h

////////////////////////////////////////////////////////////////////////////////
//                             Stage 1 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided Function stubs:

struct map *create_map(char *name, int win_requirement) {
    struct map *new_map = malloc(sizeof(struct map));
    if (new_map == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    // Copy name and initialize map fields
    strncpy(new_map->name, name, MAX_STR_LEN);
    new_map->win_requirement = win_requirement;

    // Initially, no dungeons in the map
    new_map->entrance = NULL;

    // Player not added yet
    new_map->player = NULL;

    return new_map;
}

struct player *create_player(char *name, char *class_type) {
    struct player *new_player = malloc(sizeof(struct player));
    if (new_player == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(new_player->name, name, MAX_STR_LEN);
    strncpy(new_player->class_type, class_type, MAX_STR_LEN);

    if (strcmp(class_type, "Fighter") == 0) {
        new_player->health_points = 30;
        new_player->shield_power = 2;
        new_player->damage = 8;
        new_player->magic_modifier = 0.9;
    } else if (strcmp(class_type, "Wizard") == 0) {
        new_player->health_points = 15;
        new_player->shield_power = 0;
        new_player->damage = 7;
        new_player->magic_modifier = 1.5;
    }
    
    // Initially, no items in the inventory
    new_player->inventory = NULL;
    new_player->points = 0;
    return new_player;
}

// Creates a new dungeon
//
// Parameters:
//      monster         - type of monster occupying the dungeon
//      num_monsters    - number of monsters occupying the dungeon
//      contains_player - 1 if the player is contained in this dungeon,
//                        0 otherwise
// Returns:
//      pointer to newly created struct dungeon
//
struct dungeon *create_dungeon(char *name, enum monster_type monster,
                               int num_monsters, int contains_player) {
    struct dungeon *new_dungeon = malloc(sizeof(struct dungeon));
    if (new_dungeon == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(new_dungeon->name, name, MAX_STR_LEN);
    new_dungeon->monster = monster;
    new_dungeon->num_monsters = num_monsters;
    new_dungeon->contains_player = contains_player;
    // No boss initially
    new_dungeon->boss = NULL;
    // No items initially
    new_dungeon->items = NULL;
    // No next dungeon yet
    new_dungeon->next = NULL;
    return new_dungeon;
}

int append_dungeon(struct map *map, char *name, enum monster_type monster,
                   int num_monsters) {
    // Check for duplicate name
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (strncmp(current->name, name, MAX_STR_LEN) == 0) {
            return INVALID_NAME;
        }
        current = current->next;
    }

    // Check for valid monster type
    if (monster != SLIME && monster != GOBLIN &&
        monster != SKELETON && monster != WOLF) {
        return INVALID_MONSTER;
    }

    // Check for valid number of monsters
    if (num_monsters < MIN_MONSTERS || num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    // Determine if this is the first dungeon
    int contains_player = (map->entrance == NULL) ? 1 : 0;

    // Create the new dungeon
    struct dungeon *new_dungeon = create_dungeon(name, monster, num_monsters,
                                                 contains_player);
    if (map->entrance == NULL) {
        map->entrance = new_dungeon;
    } else {
        current = map->entrance;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_dungeon;
    }

    return VALID;
}

void print_map(struct map *map) {
    // Check if the map is empty
    if (map->entrance == NULL) {
        print_empty_map();
        return;
    }

    // Print the map name
    print_map_name(map->name);

    // Traverse the list of dungeons
    struct dungeon *current = map->entrance;
    int position = 1;

    while (current != NULL) {
        // Print basic details of the current dungeon
        print_basic_dungeon(current, map->player->name, position);

        // Print connection only if there's another dungeon
        if (current->next != NULL) {
            print_connection();
        }

        // Move to the next dungeon and update position
        current = current->next;
        position++;
    }
}

// Creates a new boss
//
// Parameters:
//      health_points   - starting health points of the final boss monster
//      damage          - damage dealt by final boss monster in one turn
//      points          - when the boss is defeated, the player will gain
//                        this many points
//      required_item   - the player needs this type of item in their inventory
//                        in order to fight the boss each turn
// Returns:
//      pointer to newly created struct boss
//
struct boss *create_boss(int health_points, int damage, int points, 
                         enum item_type required_item) {
    struct boss *new_boss = malloc(sizeof(struct boss));
    if (new_boss == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    new_boss->health_points = health_points;
    new_boss->damage = damage;
    new_boss->points = points;
    new_boss->required_item = required_item;
    return new_boss;
}

int final_boss(struct map *map, enum item_type required_item) {
    // Check for valid item type
    if (required_item != PHYSICAL_WEAPON && required_item != MAGICAL_TOME &&
        required_item != ARMOR && required_item != HEALTH_POTION &&
        required_item != TREASURE) {
        return INVALID_ITEM;
    }

    // Find the last dungeon
    struct dungeon *current = map->entrance;
    while (current->next != NULL) {
        current = current->next;
    }

    // Create and assign the boss
    current->boss = create_boss(35, 10, 20, required_item);
    return VALID;
}

void player_stats(struct map *map) {
    struct dungeon *current = map->entrance;
    char *dungeon_name = NULL;

    // Traverse the dungeons to find the one containing the player
    while (current != NULL) {
        if (current->contains_player) {
            dungeon_name = current->name;
            break;
        }
        current = current->next;
    }

    // Print player stats and inventory
    print_player(map->player, dungeon_name);

    if (map->player->inventory == NULL) {
        print_no_items();
    } else {
        struct item *current_item = map->player->inventory;
        int position = 1;
        while (current_item != NULL) {
            print_item(current_item, position);
            current_item = current_item->next;
            position++;
        }
    }
}

// Your functions go here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                             Stage 2 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

int insert_dungeon(struct map *map, char *name, enum monster_type monster,
                   int num_monsters, int position) {
    // Check for invalid position
    if (position < 1) {
        return INVALID_POSITION;
    }

    // Check for duplicate name
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (strncmp(current->name, name, MAX_STR_LEN) == 0) {
            return INVALID_NAME;
        }
        current = current->next;
    }

    // Check for valid monster type
    if (monster != SLIME && monster != GOBLIN &&
        monster != SKELETON && monster != WOLF) {
        return INVALID_MONSTER;
    }

    // Check for valid number of monsters
    if (num_monsters < MIN_MONSTERS || num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    // Create the new dungeon
    struct dungeon *new_dungeon = create_dungeon(name, monster, num_monsters, 0);

    // Special case: inserting at the head (position 1)
    if (position == 1) {
        new_dungeon->next = map->entrance;
        map->entrance = new_dungeon;
        // Player starts in the entrance
        new_dungeon->contains_player = 1;
        
        // Update the previous entrance to not contain the player
        if (new_dungeon->next != NULL) {
            new_dungeon->next->contains_player = 0;
        }
        return VALID;
    }

    // Traverse to the desired position or the end of the list
    struct dungeon *prev = NULL;
    current = map->entrance;
    int current_position = 1;
    while (current != NULL && current_position < position) {
        prev = current;
        current = current->next;
        current_position++;
    }

    // Insert the new dungeon
    new_dungeon->next = current;
    if (prev != NULL) {
        prev->next = new_dungeon;
    }

    return VALID;
}

void print_dungeon(struct map *map) {
    struct dungeon *current = map->entrance;

    // Traverse the dungeons to find the one containing the player
    while (current != NULL) {
        if (current->contains_player) {
            break;
        }
        current = current->next;
    }

    // Check if the dungeon containing the player was found
    if (current == NULL) {
        printf("Player is not in any dungeon.\n");
        return;
    }

    // Print the details of the current dungeon
    print_detail_dungeon(map->player->name, current);

    // Print items if any are available
    struct item *current_item = current->items;
    int position = 1;
    if (current_item == NULL) {
        print_no_items();
    } else {
        while (current_item != NULL) {
            print_item(current_item, position);
            current_item = current_item->next;
            position++;
        }
    }
}

int move_player(struct map *map, char command) {
    struct dungeon *current = map->entrance;
    struct dungeon *previous = NULL;

    // Find the current dungeon containing the player
    while (current != NULL && !current->contains_player) {
        previous = current;
        current = current->next;
    }

    // If the player is not in any dungeon, return INVALID
    if (current == NULL) {
        return INVALID;
    }

    if (command == '>') {
        // Move to the next dungeon
        if (current->next != NULL) {
            current->contains_player = 0;
            current->next->contains_player = 1;
            return VALID;
        }
    } else if (command == '<') {
        // Move to the previous dungeon
        if (previous != NULL) {
            current->contains_player = 0;
            previous->contains_player = 1;
            return VALID;
        }
    }

    // If there is no adjacent dungeon to move into, return INVALID
    return INVALID;
}

int fight(struct map *map, char command) {
    // TODO: implement this function
    printf("Fight not yet implemented.\n");
    exit(1);
}

int end_turn(struct map *map) {
    // TODO: implement this function
    return CONTINUE_GAME;
}

int class_power(struct map *map) {
    // TODO: implement this function
    printf("Class Power not yet implemented.\n");
    exit(1);
}

// Your functions go here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                             Stage 3 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

struct item *create_item(enum item_type type, int points) {
    // TODO: implement this function
    printf("Create Item not yet implemented.\n");
    exit(1);
}

int add_item(struct map *map,
             int dungeon_number,
             enum item_type type,
             int points) {
    // TODO: implement this function
    printf("Add Item not yet implemented.\n");
    exit(1);
}

int collect_item(struct map *map, int item_number) {
    // TODO: implement this function
    printf("Collect Item not yet implemented.\n");
    exit(1);
}

int use_item(struct map *map, int item_number) {
    // TODO: implement this function
    printf("Use Item not yet implemented.\n");
    exit(1);
}

void free_map(struct map *map) {
    // TODO: implement this function
}

// Your functions here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                             Stage 4 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

int teleport(struct map *map) {
    // TODO: implement this function
    printf("Teleport not yet implemented.\n");
    exit(1);
}

int boss_fight(struct map *map) {
    // TODO: implement this function
    printf("Boss Fight not yet implemented.\n");
    exit(1);
}

// Your functions here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                              Helper Functions                              //
////////////////////////////////////////////////////////////////////////////////

// The following are helper functions that you can use to print any output
// that main.c cannot handle for you.
// The specification will tell you which functions you should use for each 
// stage.

char *get_player_name(struct player *player) {
    return player->name;
}

void print_map_name(char *name) {
    printf("Map of %s!\n", name);
}

void print_basic_dungeon(struct dungeon *dungeon, char *player_name,
                         int position) {
    printf("|^|^|^|^|^|   |^|^|^|^|^|\n\n");

    printf("%d. %s\n", position, dungeon->name);

    if (dungeon->boss == NULL) {
        printf("Boss: None\n");
    } else {
        printf("Boss: Present\n");
    }

    printf("Monster: %s\n", monster_string(dungeon->monster));

    if (dungeon->contains_player) {
        printf("%s is here\n", player_name);
    } else {
        printf("Empty\n");
    }

    printf("\n|^|^|^|^|^|   |^|^|^|^|^|\n");
}

void print_connection(void) {
    printf("          |   |\n          |   |\n          |   |\n");
}

void print_player(struct player *player, char *dungeon_name) {
    printf("=======Player Stats=======\n");
    if (dungeon_name == NULL) {
        printf("%s has not entered the map yet!\n", player->name);
    } else {
        printf("%s is currently in %s\n", player->name, dungeon_name);
    }

    printf("%s\n", class_string(player));

    printf("Health Points: %d\n", player->health_points);
    printf("Shield Power: %d\n", player->shield_power);
    printf("Damage: %d\n", player->damage);
    printf("Magic Modifier: %.1lf\n", player->magic_modifier);
    printf("Points Collected: %d\n", player->points);
    printf("%s has the following items in their inventory:\n", player->name);
}

void print_item(struct item *item, int position) {
    printf("%d. ", position);
    printf("%s", item_string(item->type));
    printf(", worth %d point(s).\n", item->points);
}

void print_no_items() {
    printf("No Items\n");
}

void print_detail_dungeon(char *player_name, struct dungeon *dungeon) {
    printf("======Dungeon Details======\n");
    printf("%s is currently in %s\n", player_name, dungeon->name);

    if (dungeon->monster == WOLF) {
        printf("There are %d wolves\n", dungeon->num_monsters);
    } else {
        printf("There are %d %ss\n", dungeon->num_monsters,
                                      monster_string(dungeon->monster));
    }

    if (dungeon->boss == NULL) {
        printf("No boss in this dungeon\n");
    } else {
        printf("The boss is in this dungeon\n");
        printf("\tHealth Points: %d\n", dungeon->boss->health_points);
        printf("\tDamage: %d\n", dungeon->boss->damage);
        printf("\tPoints: %d\n", dungeon->boss->points);
        printf("\tRequired Item: %s\n", 
               item_string(dungeon->boss->required_item));
    }

    printf("The dungeon %s has the following items:\n", dungeon->name);
}

char *class_string(struct player *player) {
    if (strncmp(player->class_type, "Fighter", 8) == 0) {
        return "Fighter";
    } else if (strncmp(player->class_type, "Wizard", 7) == 0) {
        return "Wizard";
    } else {
        return "Unknown Class";
    }
}

char *item_string(enum item_type item) {
    if (item == PHYSICAL_WEAPON) {
        return "Physical Weapon";
    } else if (item == MAGICAL_TOME) {
        return "Magical Tome";
    } else if (item == ARMOR) {
        return "Armor";
    } else if (item == HEALTH_POTION) {
        return "Health Potion";
    } else if (item == TREASURE) {
        return "Treasure";
    } else {
        return "Unknown Item";
    }
}

char *monster_string(enum monster_type monster) {
    if (monster == SLIME) {
        return "Slime";
    } else if (monster == GOBLIN) {
        return "Goblin";
    } else if (monster == SKELETON) {
        return "Skeleton";
    } else if (monster == WOLF) {
        return "Wolf";
    } else {
        return "Unknown Monster";
    }
}

void print_boss_defeat() {
    printf("The boss has been defeated!\n");
}

void print_empty_map() {
    printf("There are no dungeons currently in the dungeon.\n");
}
