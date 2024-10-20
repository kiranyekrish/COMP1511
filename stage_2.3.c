Stage 2.3: Movement Collision and Refilling Oxygen
Now when we move the player, we will have to assert that the target tile can be moved onto. A tile can be moved onto if it is on the board and there is not a rock or the lander on it already.
We will also add the ability to refill the player's oxygen level back to the full tank capacity at the lander. The tank is refilled if the player ends their turn in one of the 8 tiles adjacent to the lander.
In this substage, you will need to do the following:
1.	Check whether the tile in the corresponding direction is free to walk on before moving.
o	Tiles off the board, tiles containing the lander or tiles with rocks cannot be moved onto.
o	If they can, the player moves to the target tile.
2.	If the player moved, decrement their oxygen tank level by the current oxygen rate (starts at BASE_OXY_RATE).
3.	If the player is within 1 tile of the lander in both the vertical and horizontal directions (i.e. in one of the eight tiles directly surrounding the lander), their oxygen level is refilled to the tank capacity.
Clarifications
•	We will handle the case when the player has no oxygen in stage 3.1 when win/lose conditions are implemented, this case will not be tested until then.
Examples:
Example 2.3.1: Move Boundaries and Refill
Input:
2 2
r 0 3
q
3
100
0 0
w
a
d
d
d
s
s
[ctrl+d]
Input and Output:
dcc cs_moonlander.c -o cs_moonlander
./cs_moonlander
Please enter the [row] [col] of the lander: 2 2
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 0.00 / 0.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
Please enter cheese and rock locations:
r 0 3
q
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 0.00 / 0.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
Please enter the target qty of cheese: 3
Please enter the player's oxygen tank capacity: 100
Please enter the [row] [col] of the player: 0 0
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|0.0|   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 100.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
w
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|0.0|   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 100.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
a
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|0.0|   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 100.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
d
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |0.0|   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 99.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
d
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |0.0|^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 98.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
d
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |0.0|^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 98.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
s
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |0.0|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 100.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+
s
+---+---+---+---+---+---+---+---+---+---+
|       C S   M O O N L A N D E R       |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |^^^|   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |0.0|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |/|\|   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+---+---+
Player Cheese: 0     Lander Cheese: 0
Oxy: 100.00 / 100.00  @  1.000000 / move
+---+---+---+---+---+---+---+---+---+---+

