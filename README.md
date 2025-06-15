# Pac-Man Game

### Project Overview

This project implements a classic Pac-Man game where players can control Pac-Man to move on the game board and eat beans. The game offers both manual and automatic control modes, supporting multiple automatic movement strategies such as random movement, zigzag movement, and depth-first search (DFS) movement. Additionally, the game features functions like restarting, getting help, and exiting.

### Project Structure

The project mainly consists of the following parts:

1. **main.c**: The entry point of the program, responsible for initializing the game state and graphical interface, and starting the main loop.
2. **callback.c and callback.h**: Handle various user interaction events, such as button clicks, keyboard key presses, and timer callbacks.
3. **modele.c and modele.h**: Define the core logic of the game, including game state initialization, resource release, random placement of beans, and Pac-Man movement.
4. **`vue.c` and `vue.h`**: Responsible for drawing and updating the graphical interface.
5. **Makefile**: Used for compiling and building the project.

### Main Features

1. **Manual Control**: Players can use the keyboard (W, S, A, D) or the up, down, left, and right buttons on the interface to control Pac-Man's movement.
2. **Automatic Control**: Supports three automatic movement modes, which can be activated or deactivated via corresponding buttons:
   - **Random Movement**: Pac-Man randomly selects a direction to move.
   - **Zigzag Movement**: Pac-Man moves on the game board in a zigzag pattern.
   - **DFS Movement**: Using the depth-first search algorithm, Pac-Man intelligently finds and moves towards the nearest bean.
3. **Restart**: Clicking the "Restart" button resets the game state while retaining the current size of the game board.
4. **Help Information**: Clicking the "Help" button displays a dialog box with game instructions.
5. **Exit Game**: Clicking the "Quit" button or pressing the Q key on the keyboard releases resources and exits the game.

### Technical Implementation

1. **Graphical Interface**: The `libsx` library is used to create a graphical user interface (GUI), implementing components such as buttons and drawing areas.
2. **Game Logic**: The `GameState` structure manages the game state, including the size of the game board, Pac-Man's position, the number of moves, the number of eaten beans, etc.
3. **Random Number Generation**: The `srand()` and `rand()` functions are used to randomly place beans and select random movement directions.
4. **Depth-First Search (DFS)**: The `move_pacman_dfs()` function uses the DFS algorithm to find the nearest bean and guide Pac-Man towards it.

### Compilation and Execution

The project uses a `Makefile` for compilation. Simply execute the following command in the project's root directory:

```sh
make
```

After successful compilation, run the generated executable file and pass the number of rows and columns of the game board as parameters:

```sh
./packman <rows> <cols>
```

Here, `<rows>` and `<cols>` represent the number of rows and columns of the game board, respectively.

### Notes

- The project depends on the `libsx` library. Ensure that this library is correctly installed before compilation and execution.
- The size of the game board is specified by command-line parameters. Make sure the entered number of rows and columns are valid positive integers.
