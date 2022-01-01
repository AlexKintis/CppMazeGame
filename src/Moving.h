#ifndef __MOVING_H_
#define __MOVING_H_

#include "Colors.h"

#include "Engine.h"

#include <curses.h>
#include <memory>
#include <random>

#define TEMP_MAP_FILE "TEMP_MAP.txt"

#define POTTER_ID -1
#define GNOME_ID -2
#define TRAAL_ID -3
#define DIAMOND_ID -4
#define SPACE_ID 0
#define WALL_ID -8

typedef std::unique_ptr<std::unique_ptr<int[]>[]> unique2D;

class Moving {

    protected:
        Engine *engine;

        char entityChar;
        char eatingHabits;

        int color;

        int currY;
        int currX;

        int prevY;
        int prevX;

        // Temporary map to draw the path
        unique2D tempMap;

    public:
        Moving(Engine *);

        /* Getters */
        int getCurrentY();
        int getCurrentX();
        int getPreviousY();
        int getPreviousX();
        char getEntityChar();
        int getColor();

        // Moving
        void moveUp();
        void moveDown();
        void moveLeft();
        void moveRight();

        // Able to move to specific place
        virtual bool canMove(int, int);

        // Can eat
        virtual bool canEat(int, int);

        // Gnome and traal
        virtual int calcDistance(int, int);
        virtual void move(int, int);

        // Temporary Map
        void initiateTemporaryMap();
        void writeTemporaryMapToFile();
};

#endif // __MOVING_H_
