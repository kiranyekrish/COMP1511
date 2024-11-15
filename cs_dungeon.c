// Assignment 2 24T3 COMP1511: CS Dungeon
// cs_dungeon.c
//
// This program was written by Kiran Krish (z5600259)
// on 12-11-2024
//
// Version 1.0.0: Assignment released.
//
// This program simulates a dungeon exploration game where a player navigates
// dungeons, fights monsters, collects items, and aims to defeat a final boss.
//
// Overview:
// - The program defines several structures to represent the game entities:
//   dungeons, items, players, and bosses.
// - It provides functions to create and manage these entities, including
//   adding dungeons to the map, moving the player, and handling combat.
// - The game logic includes conditions for winning or losing based on the
//   player's actions and the state of the dungeons.
//
// References:
// - The program uses standard C libraries and follows the specifications
//   provided in the assignment documentation.

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

    // a pointer to the next dungeon in the map or NULL
    struct dungeon *next;

    // whether or not the player is inside the dungeon
    int contains_player;

    // whether or not the dungeon has been teleported to
    int teleported;
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

    // whether or not the player has used their class power
    int power_used;
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

    strncpy(new_map->name, name, MAX_STR_LEN);
    new_map->win_requirement = win_requirement;
    new_map->entrance = NULL;
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

    // Initialize stats based on class type
    // Fighter: High health and damage, some shield, low magic
    if (strcmp(class_type, "Fighter") == 0) {
        new_player->health_points = 30;
        new_player->shield_power = 2;
        new_player->damage = 8;
        new_player->magic_modifier = 0.9;
        
        // Wizard: Low health, no shield, moderate damage, high magic
    } else if (strcmp(class_type, "Wizard") == 0) {
        new_player->health_points = 15;
        new_player->shield_power = 0;
        new_player->damage = 7;
        new_player->magic_modifier = 1.5;
    }
    
    new_player->inventory = NULL;
    new_player->points = 0;
    new_player->power_used = 0;
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
    new_dungeon->boss = NULL;
    new_dungeon->items = NULL;
    new_dungeon->next = NULL;
    new_dungeon->teleported = 0;
    return new_dungeon;
}

int append_dungeon(struct map *map, char *name, enum monster_type monster,
                   int num_monsters) {
    // Check for duplicate name in existing dungeons
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (strncmp(current->name, name, MAX_STR_LEN) == 0) {
            return INVALID_NAME;
        }
        current = current->next;
    }

    if (monster != SLIME && monster != GOBLIN &&
        monster != SKELETON && monster != WOLF) {
        return INVALID_MONSTER;
    }

    if (num_monsters < MIN_MONSTERS || num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    // Player starts in the first dungeon created
    int contains_player;
    if (map->entrance == NULL) {
        contains_player = 1;
    } else {
        contains_player = 0;
    }

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
    if (map->entrance == NULL) {
        print_empty_map();
        return;
    }

    print_map_name(map->name);

    struct dungeon *current = map->entrance;
    int position = 1;

    while (current != NULL) {
        print_basic_dungeon(current, map->player->name, position);

        // Only print connection if there's another dungeon after this one
        if (current->next != NULL) {
            print_connection();
        }

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
    if (required_item != PHYSICAL_WEAPON && required_item != MAGICAL_TOME &&
        required_item != ARMOR && required_item != HEALTH_POTION &&
        required_item != TREASURE) {
        return INVALID_ITEM;
    }

    struct dungeon *current = map->entrance;
    while (current->next != NULL) {
        current = current->next;
    }

    current->boss = create_boss(35, 10, 20, required_item);
    return VALID;
}

void player_stats(struct map *map) {
    struct dungeon *current = map->entrance;
    char *dungeon_name = NULL;

    // Find which dungeon currently contains the player
    while (current != NULL) {
        if (current->contains_player) {
            dungeon_name = current->name;
            break;
        }
        current = current->next;
    }

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
    if (position < 1) {
        return INVALID_POSITION;
    }

    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (strncmp(current->name, name, MAX_STR_LEN) == 0) {
            return INVALID_NAME;
        }
        current = current->next;
    }

    if (monster != SLIME && monster != GOBLIN &&
        monster != SKELETON && monster != WOLF) {
        return INVALID_MONSTER;
    }

    if (num_monsters < MIN_MONSTERS || num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    struct dungeon *new_dungeon = create_dungeon(
        name,
        monster,
        num_monsters,
        position == 1
    );

    if (position == 1) {
        new_dungeon->next = map->entrance;
        map->entrance = new_dungeon;
        if (new_dungeon->next != NULL) {
            new_dungeon->next->contains_player = 0;
        }
        return VALID;
    }

    struct dungeon *prev = NULL;
    current = map->entrance;
    int current_position = 1;
    while (current != NULL && current_position < position) {
        prev = current;
        current = current->next;
        current_position++;
    }

    new_dungeon->next = current;
    if (prev != NULL) {
        prev->next = new_dungeon;
    }
    return VALID;
}

void print_dungeon(struct map *map) {
    struct dungeon *current = map->entrance;

    // Find the dungeon containing the player
    while (current != NULL) {
        if (current->contains_player) {
            break;
        }
        current = current->next;
    }

    if (current == NULL) {
        printf("Player is not in any dungeon.\n");
        return;
    }

    print_detail_dungeon(map->player->name, current);

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

    while (current != NULL && !current->contains_player) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return INVALID;
    }

    if (command == '>') {
        if (current->next != NULL) {
            current->contains_player = 0;
            current->next->contains_player = 1;
            return VALID;
        }
    } else if (command == '<') {
        if (previous != NULL) {
            current->contains_player = 0;
            previous->contains_player = 1;
            return VALID;
        }
    }

    return INVALID;
}

int fight(struct map *map, char command) {
    struct dungeon *current_dungeon = map->entrance;

    while (current_dungeon != NULL && !current_dungeon->contains_player) {
        current_dungeon = current_dungeon->next;
    }

    if (current_dungeon == NULL || current_dungeon->num_monsters == 0) {
        return INVALID;
    }

    int attack_damage;
    if (command == PHYSICAL_ATTACK) {
        attack_damage = map->player->damage;
    } else if (command == MAGICAL_ATTACK) {
        attack_damage = map->player->damage * map->player->magic_modifier;
    } else {
        return INVALID;
    }

    int monster_health = current_dungeon->monster;
    int monsters_defeated = 0;

    while (attack_damage >= monster_health && 
           current_dungeon->num_monsters > 0) {
        attack_damage -= monster_health;
        current_dungeon->num_monsters--;
        monsters_defeated++;
    }

    map->player->points += monsters_defeated * monster_health;

    // Use 2 to indicate monsters have been attacked
    if (current_dungeon->num_monsters > 0) {
        current_dungeon->contains_player = 2;
    }

    return VALID;
}

int check_all_monsters_defeated(struct map *map) {
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (current->num_monsters > 0) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

int check_win_conditions(struct map *map, struct dungeon *current_dungeon) {
    if (map->player->points >= map->win_requirement) {
        if (check_all_monsters_defeated(map)) {
            return WON_MONSTERS;
        }

        if (current_dungeon != NULL && 
            current_dungeon->boss != NULL && 
            current_dungeon->boss->health_points <= 0) {
            return WON_BOSS;
        }
    }
    return CONTINUE_GAME;
}

int handle_monster_attacks(struct map *map, struct dungeon *current_dungeon) {
    if (current_dungeon == NULL) {
        return CONTINUE_GAME;
    }

    int num_monsters = current_dungeon->num_monsters;
    int monster_damage = current_dungeon->monster;
    int total_monster_damage = num_monsters * monster_damage;
    // contains_player == 2 indicates monsters were attacked this turn
    int has_attacked = (current_dungeon->contains_player == 2);
    int monsters_remain = current_dungeon->num_monsters > 0;
    
    // Wolves always attack, other monsters only attack after being attacked
    if (current_dungeon->monster == WOLF || (has_attacked && monsters_remain)) {
        int effective_damage = total_monster_damage - map->player->shield_power;
        if (effective_damage < 0) {
            effective_damage = 0;
        }

        map->player->health_points -= effective_damage;

        if (map->player->health_points <= 0) {
            return PLAYER_DEFEATED;
        }
    }
    
    return CONTINUE_GAME;
}

int is_dungeon_empty(struct dungeon *dungeon) {
    return !dungeon->contains_player && 
           dungeon->num_monsters == 0 && 
           dungeon->items == NULL &&
           (dungeon->boss == NULL || dungeon->boss->health_points <= 0);
}

void remove_empty_dungeons(struct map *map) {
    struct dungeon *prev = NULL;
    struct dungeon *current = map->entrance;
    
    while (current != NULL) {
        struct dungeon *next = current->next;
        
        if (is_dungeon_empty(current)) {
            if (current->boss != NULL) {
                free(current->boss);
            }
            
            if (prev == NULL) {
                map->entrance = next;
            } else {
                prev->next = next;
            }
            
            free(current);
            current = next;
            continue;
        }
        
        prev = current;
        current = next;
    }
}

int handle_boss_combat(struct boss *boss, struct player *player) {
    int damage = boss->damage;
    // Enrage at 50% health
    if (boss->health_points <= 17) {
        damage = (int)(damage * 1.5);
    }
    int effective_damage = damage - player->shield_power;
    if (effective_damage > 0) {
        player->health_points -= effective_damage;
    }
    
    if (player->health_points <= 0) {
        return PLAYER_DEFEATED;
    }
    return CONTINUE_GAME;
}

void move_boss_towards_player(struct dungeon *boss_dungeon, int boss_pos, 
                            int player_pos, struct map *map) {
    struct dungeon *next = boss_dungeon->next;
    struct dungeon *prev = NULL;
    struct dungeon *current = map->entrance;
    
    while (current != boss_dungeon && current != NULL) {
        prev = current;
        current = current->next;
    }

    struct boss *temp_boss = boss_dungeon->boss;
    boss_dungeon->boss = NULL;
    
    if (boss_pos < player_pos && next != NULL) {
        next->boss = temp_boss;
    } else if (boss_pos > player_pos && prev != NULL) {
        prev->boss = temp_boss;
    } else {
        boss_dungeon->boss = temp_boss;
    }
}

int handle_boss_movement(struct map *map) {
    struct dungeon *current = map->entrance;
    struct dungeon *boss_dungeon = NULL;
    struct dungeon *player_dungeon = NULL;
    int boss_pos = 0, player_pos = 0, pos = 0;

    while (current != NULL) {
        pos++;
        if (current->boss != NULL && current->boss->health_points > 0) {
            boss_dungeon = current;
            boss_pos = pos;
        }
        if (current->contains_player) {
            player_dungeon = current;
            player_pos = pos;
        }
        current = current->next;
    }

    if (boss_dungeon != NULL && boss_dungeon->boss->health_points < 35) {
        if (boss_dungeon == player_dungeon) {
            return handle_boss_combat(boss_dungeon->boss, map->player);
        } else {
            move_boss_towards_player(boss_dungeon, boss_pos, player_pos, map);
        }
    }

    if (map->player->health_points <= 0) {
        return PLAYER_DEFEATED;
    }
    return CONTINUE_GAME;
}

int end_turn(struct map *map) {
    struct dungeon *current_dungeon = map->entrance;
    while (current_dungeon != NULL && !current_dungeon->contains_player) {
        current_dungeon = current_dungeon->next;
    }

    // Turn sequence:
    // 1. Player action (already happened before this function)
    // 2. Monster attacks
    int attack_status = handle_monster_attacks(map, current_dungeon);
    if (attack_status != CONTINUE_GAME) {
        return attack_status;
    }

    // 3. Clean up empty dungeons
    remove_empty_dungeons(map);

    // 4. Boss movement/attack phase
    int boss_status = handle_boss_movement(map);
    if (boss_status != CONTINUE_GAME) {
        return boss_status;
    }

    // 5. Check victory conditions
    struct dungeon *boss_dungeon = map->entrance;
    while (boss_dungeon != NULL) {
        // If boss was just defeated, award points and clean up
        if (boss_dungeon->boss != NULL && 
            boss_dungeon->boss->health_points <= 0) {
            print_boss_defeat();
            map->player->points += boss_dungeon->boss->points;
            free(boss_dungeon->boss);
            boss_dungeon->boss = NULL;
            return WON_BOSS;
        }
        boss_dungeon = boss_dungeon->next;
    }

    return check_win_conditions(map, current_dungeon);
}

int class_power(struct map *map) {
    if (map->player->power_used) {
        return INVALID;
    }

    struct dungeon *current = map->entrance;
    while (current != NULL && !current->contains_player) {
        current = current->next;
    }

    if (strcmp(map->player->class_type, "Fighter") == 0) {
        // Permanent damage boost to help with tougher enemies
        map->player->damage = (int)(map->player->damage * 1.5);
    } else if (strcmp(map->player->class_type, "Wizard") == 0) {
        // Clear room with magic but cannot affect bosses
        if (current != NULL) {
            map->player->points += current->num_monsters * current->monster;
            current->num_monsters = 0;
        }
    }

    map->player->power_used = 1;
    return VALID;
}

// Your functions go here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                             Stage 3 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

struct item *create_item(enum item_type type, int points) {
    // Dynamically allocate memory to allow for variable-length item lists
    struct item *new_item = malloc(sizeof(struct item));
    if (new_item == NULL) {
        return NULL;
    }
    new_item->type = type;
    new_item->points = points;
    return new_item;
}

int add_item(struct map *map,
             int dungeon_number,
             enum item_type type,
             int points) {
    if (dungeon_number < 1) {
        return INVALID_DUNGEON;
    }
    
    // Navigate to the target dungeon by counting positions from the entrance
    struct dungeon *current_dungeon = map->entrance;
    for (int i = 1; i < dungeon_number; i++) {
        if (current_dungeon == NULL) {
            return INVALID_DUNGEON;
        }
        current_dungeon = current_dungeon->next;
    }
    
    if (current_dungeon == NULL) {
        return INVALID_DUNGEON;
    }

    if (type < PHYSICAL_WEAPON || type > TREASURE) {
        return INVALID_ITEM;
    }
    
    if (points < MIN_ITEM_POINT_VALUE || points > MAX_ITEM_POINT_VALUE) {
        return INVALID_POINTS;
    }
    
    struct item *new_item = create_item(type, points);
    if (new_item == NULL) {
        return INVALID_ITEM;
    }
    
    // Maintain sorted order of items by type for consistent inventory display
    struct item *prev_item = NULL;
    struct item *current_item = current_dungeon->items;
    while (current_item != NULL && current_item->type <= type) {
        prev_item = current_item;
        current_item = current_item->next;
    }

    if (prev_item == NULL) {
        new_item->next = current_dungeon->items;
        current_dungeon->items = new_item;
    } else {
        new_item->next = prev_item->next;
        prev_item->next = new_item;
    }
    
    return VALID;
}

int collect_item(struct map *map, int item_number) {
    if (item_number < 1) {
        return INVALID_ITEM;
    }

    struct dungeon *current_dungeon = map->entrance;
    while (current_dungeon != NULL && !current_dungeon->contains_player) {
        current_dungeon = current_dungeon->next;
    }

    if (current_dungeon == NULL) {
        return INVALID_ITEM;
    }

    struct item *prev_item = NULL;
    struct item *current_item = current_dungeon->items;
    int current_position = 1;

    while (current_item != NULL && current_position < item_number) {
        prev_item = current_item;
        current_item = current_item->next;
        current_position++;
    }

    if (current_item == NULL) {
        return INVALID_ITEM;
    }

    // Add item to front of inventory for O(1) insertion
    if (prev_item == NULL) {
        current_dungeon->items = current_item->next;
    } else {
        prev_item->next = current_item->next;
    }
    current_item->next = map->player->inventory;
    map->player->inventory = current_item;

    return VALID;
}

int use_item(struct map *map, int item_number) {
    if (item_number < 1) {
        return INVALID_ITEM;
    }

    struct item *prev_item = NULL;
    struct item *current_item = map->player->inventory;
    int current_position = 1;

    while (current_item != NULL && current_position < item_number) {
        prev_item = current_item;
        current_item = current_item->next;
        current_position++;
    }

    if (current_item == NULL) {
        return INVALID_ITEM;
    }

    // Magical tomes increase magic modifier by 10% of points
    if (current_item->type == PHYSICAL_WEAPON) {
        map->player->damage += current_item->points;
    } else if (current_item->type == MAGICAL_TOME) {
        map->player->magic_modifier += current_item->points / 10.0;
        // Armor increases shield power by half of points
    } else if (current_item->type == ARMOR) {
        map->player->shield_power += current_item->points / 2;
    } else if (current_item->type == HEALTH_POTION) {
        map->player->health_points += current_item->points + 5;
        if (map->player->health_points > MAX_HEALTH) {
            map->player->health_points = MAX_HEALTH;
        }
    } else if (current_item->type == TREASURE) {
        // No effect on player stats
    } else {
        return INVALID_ITEM;
    }

    map->player->points += current_item->points;

    if (prev_item == NULL) {
        map->player->inventory = current_item->next;
    } else {
        prev_item->next = current_item->next;
    }
    free(current_item);

    return VALID;
}

void free_map(struct map *map) {
    if (map == NULL) {
        return;
    }

    struct dungeon *current = map->entrance;
    while (current != NULL) {
        struct dungeon *next = current->next;
        
        struct item *current_item = current->items;
        while (current_item != NULL) {
            struct item *next_item = current_item->next;
            free(current_item);
            current_item = next_item;
        }
        
        if (current->boss != NULL) {
            free(current->boss);
        }
        
        free(current);
        current = next;
    }

    if (map->player != NULL) {
        struct item *current_item = map->player->inventory;
        while (current_item != NULL) {
            struct item *next_item = current_item->next;
            free(current_item);
            current_item = next_item;
        }
        free(map->player);
    }

    free(map);
}

// Your functions here (include function comments):


////////////////////////////////////////////////////////////////////////////////
//                             Stage 4 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

int teleport(struct map *map) {
    if (map->entrance == NULL || map->entrance->next == NULL) {
        return INVALID;
    }

    struct dungeon *current = map->entrance;
    int current_pos = 1;
    while (current != NULL && !current->contains_player) {
        current = current->next;
        current_pos++;
    }

    if (current == NULL) {
        return INVALID;
    }
    // Keep current marked as visited
    current->teleported = 1;

    struct dungeon *temp = map->entrance;
    struct dungeon *furthest = NULL;
    int temp_pos = 1, max_distance = -1;

    while (temp != NULL) {
        if (!temp->teleported) {
            int distance = abs(current_pos - temp_pos);
            // If equal distance, earlier dungeon is kept due to > comparison
            if (distance > max_distance) {
                max_distance = distance;
                furthest = temp;
            }
        }
        temp = temp->next;
        temp_pos++;
    }

    if (furthest == NULL) {
        temp = map->entrance;
        while (temp != NULL) {
            temp->teleported = (temp == current);
            temp = temp->next;
        }
        return teleport(map);
    }

    current->contains_player = 0;
    furthest->contains_player = 1;
    furthest->teleported = 1;

    return VALID;
}

void swap_dungeon_properties(struct dungeon *dungeon_a, 
                           struct dungeon *dungeon_b) {
    char temp_name[MAX_STR_LEN];
    strncpy(temp_name, dungeon_a->name, MAX_STR_LEN);
    strncpy(dungeon_a->name, dungeon_b->name, MAX_STR_LEN);
    strncpy(dungeon_b->name, temp_name, MAX_STR_LEN);

    enum monster_type temp_monster = dungeon_a->monster;
    dungeon_a->monster = dungeon_b->monster;
    dungeon_b->monster = temp_monster;

    int temp_num = dungeon_a->num_monsters;
    dungeon_a->num_monsters = dungeon_b->num_monsters;
    dungeon_b->num_monsters = temp_num;

    struct boss *temp_boss = dungeon_a->boss;
    dungeon_a->boss = dungeon_b->boss;
    dungeon_b->boss = temp_boss;

    struct item *temp_items = dungeon_a->items;
    dungeon_a->items = dungeon_b->items;
    dungeon_b->items = temp_items;

    int temp_contains = dungeon_a->contains_player;
    dungeon_a->contains_player = dungeon_b->contains_player;
    dungeon_b->contains_player = temp_contains;

    dungeon_a->teleported = 0;
    dungeon_b->teleported = 0;
}

void shuffle_dungeons(struct map *map) {
    struct dungeon *current_pair = map->entrance;
    while (current_pair != NULL && current_pair->next != NULL) {
        swap_dungeon_properties(current_pair, current_pair->next);
        current_pair = current_pair->next->next;
    }
}

double calculate_player_damage(struct player *player) {
    double physical_damage = player->damage;
    double magical_damage = player->damage * player->magic_modifier;
    if (magical_damage >= physical_damage) {
        return magical_damage;
    }
    return physical_damage;
}

int boss_fight(struct map *map) {
    struct dungeon *current = map->entrance;
    while (current != NULL && !current->contains_player) {
        current = current->next;
    }

    if (current == NULL || current->boss == NULL) {
        return NO_BOSS;
    }

    struct item *inventory = map->player->inventory;
    int has_required_item = 0;
    while (inventory != NULL) {
        if (inventory->type == current->boss->required_item) {
            has_required_item = 1;
            break;
        }
        inventory = inventory->next;
    }

    if (!has_required_item) {
        return NO_ITEM;
    }

    double damage = calculate_player_damage(map->player);
    struct boss *current_boss = current->boss;
    int original_health = current_boss->health_points;
    current_boss->health_points -= damage;

    // On first hit, shuffle all adjacent dungeon pairs
    if (original_health == 35) {
        shuffle_dungeons(map);
    }

    return VALID;
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
