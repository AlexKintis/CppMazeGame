# Cpp Ncurses Maze Game

This game was an university assignment in which we had to create a C++ maze game with the [Ncurses library](https://invisible-island.net/ncurses/announce.html) and is called **Conviction of Mpampis Potereides**.

<div align="center">
<img src="https://github.com/AlexKintis/CppNCursesMazeGame/blob/main/Images/GameLogo.png" width="45%" alt="Main window">
<img src="https://github.com/AlexKintis/CppNCursesMazeGame/blob/main/Images/GamePlay.png" width="45%" alt="Chart window">
</div>

You can run this game under any OS which has installed `cmake` and `ncurses library`.

## How to run

1. Clone the repository
2. Run the following commands into the root of the project:

    ```bash
    cmake -S . -B "build"
    cmake --build "build"
    ```

3. Run the game with `./build/potereides`

## Bugfixes

- Changed the initialization of the shared_ptr in the `Engine.cpp` in order that the game can start.
