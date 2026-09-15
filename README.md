# SnakeX – Console-Based Snake Game

SnakeX is a console-based Snake Game developed in **C++** using Object-Oriented Programming concepts. The game allows players to control a snake, collect randomly generated food, increase their score, and grow the snake’s body.

The application is designed for Linux-based systems and uses terminal control techniques to support immediate keyboard input without requiring the player to press Enter after every direction key.

## Features

- Console-based Snake Game
- Real-time keyboard input
- W/A/S/D movement controls
- Random food generation
- Dynamic snake growth
- Score tracking
- Wall collision detection
- Self-collision detection
- Player name validation
- Custom grid width and height
- Easy, Medium, and Hard difficulty levels
- Restart or quit option
- Linux terminal support
- Modular class-based design

## Technologies Used

- **Language:** C++
- **Programming Paradigm:** Object-Oriented Programming
- **Compiler:** GCC
- **Operating System:** Linux
- **Libraries:**
  - `iostream`
  - `cstdlib`
  - `ctime`
  - `cctype`
  - `string`
  - `unistd.h`
  - `termios.h`

## Controls

| Key | Action |
|-----|--------|
| W | Move Up |
| A | Move Left |
| S | Move Down |
| D | Move Right |
| X | Quit the current game |
| R | Restart after Game Over |
| Q | Quit after Game Over |

The game accepts movement keys immediately without requiring the Enter key.

## How the Game Works

1. The player enters a valid name containing only alphabetic characters.
2. The player selects the grid width and height.
3. The player chooses a difficulty level.
4. The snake is placed at the center of the grid.
5. Food is generated at a random position inside the grid.
6. The player controls the snake using W, A, S, and D.
7. When the snake eats food:
   - The score increases by 10.
   - The snake grows by one segment.
   - New food is generated.
8. The game ends when the snake:
   - Hits a boundary.
   - Collides with its own tail.
   - Receives the quit command.
9. The player can restart the game or exit.

## Project Structure

### 1. Terminal Class

The `Terminal` class manages Linux console operations.

Its responsibilities include:

- Enabling raw terminal mode
- Disabling raw terminal mode
- Reading keyboard input immediately
- Clearing the terminal screen

### 2. Snake Class

The `Snake` class stores and manages the snake’s state.

It handles:

- Snake head coordinates
- Tail coordinates
- Snake length
- Current movement direction
- Snake movement
- Snake growth
- Tail collision checking

### 3. Food Class

The `Food` class manages food generation.

It is responsible for:

- Storing food coordinates
- Generating random food positions
- Placing food inside the selected grid dimensions

### 4. SnakeX Class

The `SnakeX` class acts as the main game engine.

It manages:

- Player information
- Game score
- Grid dimensions
- Difficulty and speed
- Game initialization
- Menu handling
- Rendering
- Keyboard input
- Game rules
- Restart and quit functionality

## Object-Oriented Programming Concepts

This project demonstrates the following OOP concepts:

- **Classes and Objects:** The game is divided into `Terminal`, `Snake`, `Food`, and `SnakeX` classes.
- **Encapsulation:** Snake data such as position, length, tail, and direction is kept private.
- **Abstraction:** Complex operations such as movement, food spawning, and terminal handling are performed through simple member functions.
- **Modularity:** Each class has a specific responsibility, making the code easier to understand and maintain.

## Game Logic
                  

The game follows a continuous loop:

```text
Display Game Board
       ↓
Read Keyboard Input
       ↓
Update Snake Position
       ↓
Check Boundary Collision
       ↓
Check Self-Collision
       ↓
Check Food Collision
       ↓
Update Score and Length
       ↓
Repeat Until Game Over
```

Input Handling

The program uses Linux terminal functionality through the termios library.

Raw mode disables:

Canonical input mode, which normally waits for Enter
Keyboard echo, which prevents typed characters from appearing on the screen

This allows the game to read movement keys immediately using the read() function.

Difficulty Levels
Difficulty	Speed	Description
Easy	Slower movement	Suitable for beginners
Medium	Normal movement	Balanced gameplay
Hard	Faster movement	More challenging gameplay
Requirements
Linux operating system
GCC or another C++ compiler
C++11 or later
Standard keyboard
Terminal or GDB terminal
Compilation

Open a terminal in the project directory and run:

g++ -std=c++11 -Wall -Wextra snake.cpp -o snake
Running the Game

Run the compiled program using:

./snake

To run it through GDB:

g++ -g -std=c++11 snake.cpp -o snake
gdb ./snake

Inside GDB, execute:

run
Limitations
The game is console-based.
The maximum snake tail length is limited to 100 segments.
Scores are not permanently stored.
The game supports only one player.
There are no graphical animations or sound effects.
The current version does not include obstacles or power-ups.
Food can be generated at a position occupied by the snake because the current food-generation method selects a random coordinate without checking the snake’s body.
Future Enhancements
Add a graphical interface using SFML or SDL2.
Implement persistent high-score storage using files or SQLite.
Add obstacles and power-ups.
Add pause and resume functionality.
Add multiple levels and advanced difficulty settings.
Add sound effects and animations.
Implement multiplayer functionality.
Improve food generation to prevent food from appearing on the snake’s body.
Learning Outcomes

Through this project, the following concepts were practiced:

C++ classes and objects
Encapsulation and abstraction
Arrays and coordinate management
Enumerations
Loops and conditional statements
Random number generation
Terminal input handling
Collision detection
Game-loop implementation
Debugging and problem-solving
Author

Yash Detani

B.Tech Computer Science and Engineering
VIT Bhopal University

License

This project is created for educational and academic purposes.
