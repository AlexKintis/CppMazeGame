#ifndef __ENGINE_H_
#define __ENGINE_H_

#include "Colors.h"
#include "HiScore.h"

#include <curses.h>
#include <memory>
#include <vector>

typedef std::shared_ptr<std::shared_ptr<char[]>[]> shared2D;

#define ROOT_WIN_Y mapY + 8
#define ROOT_WIN_X mapX + 12

#define WALL "█"

class Engine {

    private:
        WINDOW *rootWin;
        WINDOW *mapWin;

        // Map table
        shared2D map;
        int mapY;
        int mapX;

        std::shared_ptr<int[]> tempXY;

        int keyPressCode;

    public:
        /* Constructors */
        Engine(const char *);

        /* Getters */
        WINDOW *getRootWin();
        WINDOW *getMapWin();
        int getCharFromMap(int, int);
        int *getMapXY();
        int *getRandomizedYX();
        int getKeyCode();

        /* Methods */
        void initScreen();
        void initColors();

        // Map
        shared2D getMap();
        int getX();
        int getY();
        void loadMapFromFile(const char *);
        void printMap(); // !!Not at Ncurses

        // WINDOW
        void drawMap();
        void drawRootWin();
        void drawMapWin();
        void drawArtWork(std::vector<std::vector<char>>);
        void drawScore(int);
        void refreshRootWin();
        void refreshMapWin();
        void refreshArtWorkWin();
        void clearMapWindow();
        std::string askForUserName();
        void displayScores(HiScore);

        // Entities
        void spawnEntityAtMap(char, int, int, int);
        void moveEntityAtMap(char, int, int, int, int, int);
        int *spawnPlaceRandomizer();

        // Keys
        void captureKeyPress();
        bool escKeyPressed();

        // Destroy Screen
        void destroyScreen();
};

#endif // __ENGINE_H_
