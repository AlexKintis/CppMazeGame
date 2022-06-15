#include "Engine.h"
#include "Colors.h"

#include <iostream>
#include <cstring>
#include <memory>
#include <sstream>
#include <fstream>
#include <curses.h>
#include <string>
#include <vector>
#include <wchar.h>

/* Constuctors */
Engine::Engine(const char *mapFilePath) {

    this-> map = NULL;
    this-> mapX = 0;
    this-> mapY = 0;
    this-> loadMapFromFile(mapFilePath);

    this-> keyPressCode = 0;
    this-> tempXY = std::shared_ptr<int[]>(new int[2]);
    srand(time(NULL));
}

/* Getters */
WINDOW *Engine::getRootWin() { return this-> rootWin; }

WINDOW *Engine::getMapWin() { return this-> mapWin; }

int Engine::getCharFromMap(int Y, int X) {

    return mvwinch(this-> mapWin, Y, X) & A_CHARTEXT; // Remove ascii color
}

int *Engine::getMapXY() {

    getmaxyx(this-> mapWin, this-> tempXY[0], this-> tempXY[1]);

    return this-> tempXY.get();
}

int *Engine::getRandomizedYX() {
    return this-> tempXY.get();
}

int Engine::getKeyCode() { return this-> keyPressCode; }

// Map
shared2D Engine::getMap() {
    return this-> map;
}

int Engine::getX() {
    return this-> mapX;

}

int Engine::getY() {
    return this-> mapY;
}

void Engine::loadMapFromFile(const char *filePath) {

    try {
        /* begin Reading file */
        std::ifstream mapFile(filePath, std::fstream::in);

        char c;

        if(!mapFile.is_open()) {
            throw "File opening error!";
        }

        bool firstLine = false;

        while(mapFile.get(c)) {
            if(c != '\n') {
                if(!firstLine) {
                    this-> mapY++;
                }
            } else {
                this-> mapX++;
                firstLine = true;
            }
        }

        /* dynamically allocate map variable */
        this-> map = shared2D(new std::shared_ptr<char[]>[this-> mapX]);

        for(unsigned int i = 0; i < this-> mapX; i++) {
            this-> map[i] = std::unique_ptr<char[]>(new char[this-> mapY]);
        }

        // Go to begin of file
        mapFile.clear();
        mapFile.seekg(0, std::ios::beg);

        /* Read map from file */
        int x = 0, y = 0;
        while(x < this-> mapX) {
            c = mapFile.get();
            if(y == this->mapY) {
                y = 0;
                x++;
            } else {
                if(c != '\n') {
                    this-> map[x][y++] = c;
                }
            }
        }

        int temp = this-> mapX;
        this-> mapX = this-> mapY;
        this-> mapY = temp;

        mapFile.close();

    } catch(char *msg) {
        std::cout << msg << std::endl;
        return;
    }
}

void Engine::printMap() {
    for(unsigned int i = 0; i < this-> mapX; i++) {
        for(unsigned int j = 0; j < this-> mapY; j++) {
            std::cout << this-> map[i][j];
        }
        std::cout << std::endl;
    }
}

/* Methods */
void Engine::initScreen() {

    // getmaxyx(stdscr, this-> termYmax, this-> termXmax);

    initscr();

    initColors();

    cbreak();
    noecho();
    nonl();
    intrflush (stdscr, FALSE);
    keypad (stdscr, TRUE);
    curs_set(0);

    clear(); // Clear screen

    refresh();

    /* Root Window */
    this-> rootWin = newwin(ROOT_WIN_Y, ROOT_WIN_X, 0, 0);
    drawRootWin();

    /* Map Window */
    this-> mapWin = subwin(this-> rootWin, mapY + 2, mapX + 2, 3, 5);
    drawMapWin();

}

void Engine::initColors() {
    if (has_colors() == TRUE && USE_COLORS) {
        start_color ();

        // Init Colors
        init_color(COLOR_BLACK, 125, 125, 125);
        init_color(COLOR_RED, 645, 13, 76);
        init_color(COLOR_ORANGE, 600, 335, 0);
        init_color(COLOR_GREEN, 0, 1000, 0);
        init_color(COLOR_BRIGHT_RED, 1000, 0, 0);
        init_color(COLOR_CYAN, 0, 1000, 1000);
        init_color(COLOR_DARKER_CYAN, 0, 600, 600);
        init_color(COLOR_WHITE, 500, 500, 500);

        // Init Pairs
        init_pair(1, COLOR_RED, COLOR_RED); // Outerbox
        init_pair(2, COLOR_RED, COLOR_BLACK); // InnerBox
        init_pair(3, COLOR_RED, COLOR_BLACK); // Title
        init_pair(4, COLOR_WHITE, COLOR_BLACK); // Map
        init_pair(5, COLOR_GREEN, COLOR_BLACK); // Mpampis
        init_pair(6, COLOR_BRIGHT_RED, COLOR_BLACK); // Hostiles
        init_pair(7, COLOR_CYAN, COLOR_BLACK); // DIAMONT
    }
}

// WINDOW
void Engine::drawMap() {
    for(unsigned int x = 0; x < this-> mapY; x++) { // The loop is reversed -> x,y
        for(unsigned int y = 0; y < this-> mapX; y++) {
            wattron(this-> mapWin, COLOR_PAIR(MAP_PAIR));
            mvwprintw(this-> mapWin, x + 1, y + 1, "%s", map[x][y] == '*' ? WALL : " ");
            wattroff(this-> mapWin, COLOR_PAIR(MAP_PAIR));
        }
    }
    this-> refreshMapWin();
}

void Engine::drawRootWin() {
    wattron(this-> rootWin, COLOR_PAIR(OUTERBOX_PAIR));
    box(this-> rootWin, 0, 0);
    wattroff(this-> rootWin, COLOR_PAIR(OUTERBOX_PAIR));

    wattron(this->rootWin, COLOR_PAIR(TITLE_PAIR));
    mvwprintw(this-> rootWin, 2, mapX / 2 - 10, "CONVICTION OF MPAMPIS POTEREIDES");
    wattroff(this-> rootWin, COLOR_PAIR(TITLE_PAIR));

    this-> refreshRootWin();
}

void Engine::drawMapWin() {
    wattron(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));
    box(this-> mapWin, 0, 0);
    wattroff(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));

    this->refreshMapWin();
}

void Engine::drawArtWork(std::vector<std::vector<char>> artWork) {

    wattron(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));

    int maxMapWinY, maxMapWinX;
    getmaxyx(this-> mapWin, maxMapWinY, maxMapWinX);

    for(int y = 0; y < artWork.size(); y++){
        for(int x = 0; x < artWork[0].size(); x++) {
            mvwaddch(this-> mapWin, y + ((maxMapWinY - artWork.size()) / 2), x + ((maxMapWinX - artWork[0].size()) / 2), artWork[y][x]);
        }
    }
    wattroff(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));

    wrefresh(this-> mapWin);

    getch();

    wclear(this-> mapWin);
    this-> drawMapWin();
}

void Engine::drawScore(int score) {
    wattron(this-> rootWin, COLOR_PAIR(TITLE_PAIR));
    mvwprintw(this-> rootWin, ROOT_WIN_Y - 3, 6, "Score: %3d", score);
    wattroff(this-> rootWin, COLOR_PAIR(TITLE_PAIR));
}

void Engine::refreshRootWin() { wrefresh(this-> rootWin); }

void Engine::refreshMapWin() { wrefresh(this-> mapWin); }

void Engine::clearMapWindow() {
    wclear(this-> mapWin);

    // Draw again Box
    wattron(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));
    box(this-> mapWin, 0, 0);
    wattroff(this-> mapWin, COLOR_PAIR(INNERBOX_PAIR));

    this-> refreshMapWin();
}

std::string Engine::askForUserName() {

    std::string name;

    int maxMapWinY, maxMapWinX;
    getmaxyx(this-> mapWin, maxMapWinY, maxMapWinX);

    const std::string message = "Please add your Name";

    int Y = maxMapWinY / 2;
    int X = (maxMapWinX - message.size()) / 2;

    wattron(this-> mapWin, COLOR_PAIR(TITLE_PAIR));
    mvwprintw(this-> mapWin, Y, X, "%s", message.c_str());
    wattroff(this-> mapWin, COLOR_PAIR(TITLE_PAIR));

    this-> refreshMapWin();

    curs_set(1);
    // nocbreak();
    keypad (stdscr, false);
    echo();

    char *tempName;

    // Move cursor to accept user input
    wmove(this-> mapWin, Y+2, (X - 10) + message.size() / 2);
    wscanw(this-> mapWin, "%s", tempName);

    name = std::string(tempName);

    return name;
}

void Engine::displayScores(HiScore scores) {

    this->clearMapWindow();

    int maxMapWinY, maxMapWinX;
    getmaxyx(this-> mapWin, maxMapWinY, maxMapWinX);

    int Y = maxMapWinY / 2;
    int X = (maxMapWinX - 16) / 2;

    wattron(this-> mapWin, COLOR_PAIR(DIAMOND_PAIR));
    mvwprintw(this-> mapWin, Y - 2, X, "High Scores");
    wattroff(this-> mapWin, COLOR_PAIR(DIAMOND_PAIR));

    for(unsigned int i = 0; i < 5; i++) {
        wattron(this-> mapWin, COLOR_PAIR(POTTER_PAIR));
        mvwprintw(this-> mapWin, Y+ i, X - 2, "%-10s %3d", scores.getName(i), scores.getScore(i));
        wattroff(this-> mapWin, COLOR_PAIR(POTTER_PAIR));
    }

    this->refreshMapWin();

    getch();
}

// Entities
void Engine::spawnEntityAtMap(char entitySymbol, int Y, int X, int color) {
    wattron(this-> mapWin, COLOR_PAIR(color));
    //mvwaddstr(this-> mapWin, Y, X, entitySymbol.c_str());
    mvwaddch(this-> mapWin, Y, X, entitySymbol);
    wattroff(this-> mapWin, COLOR_PAIR(color));
    this-> refreshMapWin();
}

void Engine::moveEntityAtMap(char entitySymbol, int prevY, int prevX, int Y, int X, int color) {
    mvwaddch(this-> mapWin, prevY, prevX, ' ');
    wattron(this-> mapWin, COLOR_PAIR(color));
    // mvwaddstr(this-> mapWin, Y, X, entitySymbol.c_str());
    mvwaddch(this-> mapWin, Y, X, entitySymbol);
    wattroff(this-> mapWin, COLOR_PAIR(color));
    this-> refreshMapWin();
}

int *Engine::spawnPlaceRandomizer() {

    int maxY, maxX;

    tempXY[0] = 0;
    tempXY[1] = 0;

    getmaxyx(this-> mapWin, maxY, maxX);

    do {
        tempXY[0] = rand() % (maxY + 1);
        tempXY[1] = rand() % (maxX + 1);
    } while(getCharFromMap(tempXY[0], tempXY[1]) != ' ');

    return this-> tempXY.get();
}

// Keys
void Engine::captureKeyPress() { this-> keyPressCode = getch(); }

bool Engine::escKeyPressed() { return this-> keyPressCode == 27 ? true : false; }

// Destroy Screen
void Engine::destroyScreen() { endwin(); }
