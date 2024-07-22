2048 Game in C
This is a simple implementation of the popular 2048 game in C using the ncurses library for a text-based interface.

Table of Contents
Gameplay
How to Play
Code Overview
Contributing
License
Gameplay
2048 is a single-player sliding block puzzle game. The objective is to slide numbered tiles on a 4×4 grid to combine them and create a tile with the number 2048.
How to Play
Use the arrow keys to move the tiles.
Combine tiles with the same number to merge them into one with double the value.
The game is won when a tile with the number 2048 appears on the board.
Press 'q' to quit the game.
Code Overview
Main Functions
INIT_2048(): Initializes the ncurses library and sets up the game environment.
DONE_2048(): Cleans up the ncurses environment and exits the program.
display_board(): Displays the current state of the game board.
add_two(): Adds a tile with the value of 2 to a random empty spot on the board.
reset_board(): Resets the game board and starts a new game.
game_lost(): Checks if there are no more valid moves and the game is lost.
move_tile(): Handles the logic for moving and merging tiles.
move_board(): Executes a move in the specified direction.
get_key(): Waits for and returns a key press from the player.
game_over(): Displays the game over message and waits for a key press to restart the game.
Game Flow
Initialize the game environment using INIT_2048().
Reset the board with reset_board().
Enter the main game loop:
Get the player's move using get_key().
Move the tiles and update the board using move_board().
Add a new tile using add_two().
Display the updated board using display_board().
Check for game over or win conditions.
If the game is over, display the game over message using game_over() and wait for a key press to restart the game.
If the player wins (creates a 2048 tile), display the win message and wait for a key press to restart the game.
Contributing
If you'd like to contribute to this project, feel free to fork the repository and submit a pull request. For major changes, please open an issue first to discuss what you would like to change.
