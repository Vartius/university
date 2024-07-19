# Console Backgammon Game

## Description
This is a console-based implementation of a board game similar to Backgammon, developed using C++ and the `ncurses` library for handling the console UI. The game supports two players and includes features such as rolling dice, moving checkers, and handling player inputs.

## Features
- Two-player game
- Dice rolling
- Checker movement
- Color-coded UI
- Player name input
- Hall of Fame
- Game history
- Save and load game states

## Requirements
- C++ Compiler (GCC, Clang, etc.)
- `ncurses` library
- CMake

## Installation
1. Ensure you have a C++ compiler installed. For example, you can install GCC on Ubuntu with:
    ```sh
    sudo apt-get update
    sudo apt-get install g++
    ```
2. Install the `ncurses` library:
    ```sh
    sudo apt-get install libncurses5-dev libncursesw5-dev
    ```
3. Install CMake:
    ```sh
    sudo apt-get install cmake
    ```
4. Clone this repository or download the source code.

## Compilation
To compile the game using CMake, follow these steps:

1. Create a build directory and navigate to it:
    ```sh
    mkdir build
    cd build
    ```
2. Run CMake to configure the project:
    ```sh
    cmake ..
    ```
3. Build the project:
    ```sh
    make
    ```

## Running the Game
After compilation, you can run the game with:
```sh
./backgammon
```

## How to Play
1. Upon starting the game, you will be prompted to enter the names of Player 1 and Player 2.
2. The game board will be displayed with checkers positioned at their starting points.
3. Players take turns to roll the dice and move their checkers according to standard Backgammon rules.
4. Use the keyboard to interact with the game. Specific controls will be displayed on-screen as needed.

## Code Overview
- **CMakeLists.txt**: Configuration file for CMake.
- **include/**: Directory for header files (if any).
- **src/**: Directory containing the source code and related files.
  - `main.cpp`: Contains the main game logic, UI rendering, and player interaction handling.
  - `hallOfFame`: Stores the hall of fame data.
  - `history`: Stores the game history.
  - `saves`: Directory for saving game states.
  
  - `struct Coords`: Defines coordinates on the board.
  - `struct Board`: Represents the board state with color and count of checkers.
  - `struct Names`: Holds the names of the two players.
  - `getPos()`: Calculates the screen position for a given board coordinate.
  - `my_strcmp()` and `my_strcpy()`: Utility functions for string comparison and copying.
  - `input()`: Handles player input for names.
  - `roll()`: Simulates rolling dice.
  - `printBoard()`, `printDownSide()`, and `printCheckers()`: Functions to render different parts of the game board on the screen.

## Authors
- Matsvei Kasporvich (s201627)

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgements
- The `ncurses` library for providing a framework for console UI.
- Backgammon enthusiasts for inspiration.
