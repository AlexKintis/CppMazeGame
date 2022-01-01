#include "Moving.h"
#include "Game.h"
#include <curses.h>

#include <iostream>
#include <fstream>

/* Constructors */
Moving::Moving(Engine *engine) {
    this-> engine = engine;
    // Randomizing spawn place
    int *tempYX = this-> engine -> spawnPlaceRandomizer();
    this-> currY = tempYX[0];
    this-> currX = tempYX[1];
}

/* Getters */
int Moving::getCurrentY() { return this-> currY; }

int Moving::getCurrentX() { return this-> currX; }

int Moving::getPreviousY() { return this-> prevY; }

int Moving::getPreviousX() { return this-> prevX; }

char Moving::getEntityChar() { return this-> entityChar; }

int Moving::getColor() { return this-> color; }

// Moving
void Moving::moveUp() {

    int &Y = this-> currY;
    int &X = this-> currX;

    if ((Y > 0) && (this-> canMove(Y - 1, X) || this-> canEat(Y - 1, X))) {
        this-> prevY = Y;
        this-> prevX = X;
        Y = Y - 1;
    }
}

void Moving::moveDown() {

    int &Y = this-> currY;
    int &X = this-> currX;

    if ((Y < LINES - 1) && (canMove(Y + 1, X) || this-> canEat(Y + 1, X))) {
        this-> prevY = Y;
        this-> prevX = X;
        Y = Y + 1;
    }

}

void Moving::moveLeft() {

    int &Y = this-> currY;
    int &X = this-> currX;

    if ((X > 0) && (canMove(Y, X - 1) || this-> canEat(Y, X - 1))) {
        this-> prevY = Y;
        this-> prevX = X;
        X = X - 1;
    }
}

void Moving::moveRight() {

    int &Y = this-> currY;
    int &X = this-> currX;

    if ((X < COLS - 1) && (canMove(Y, X + 1) || this-> canEat(Y, X + 1))) {
        this-> prevY = Y;
        this-> prevX = X;
        X = X + 1;
    }
}

// Able to move
bool Moving::canMove(int Y, int X) { return false; }

// Can eat
bool Moving::canEat(int Y, int X) { return false; }

int Moving::calcDistance(int Y, int X) {
    return 0;
}

void Moving::move(int Y, int X) {

}

// Temporary Map
void Moving::initiateTemporaryMap() {

    Engine e = *(this-> engine);

    for(int i = 0; i < this->engine -> getMapXY()[0]; i++) {
        for(int j = 0; j < this->engine -> getMapXY()[1]; j++) {

            char ch = e.getCharFromMap(i, j);

            switch(ch) {
                case 'M':
                    this-> tempMap[i][j] = POTTER_ID;
                    break;
                case 'G':
                    this-> tempMap[i][j] = GNOME_ID;
                    break;
                case 'T':
                    this-> tempMap[i][j] = TRAAL_ID;
                    break;
                case 'D':
                    this-> tempMap[i][j] = DIAMOND_ID;
                    break;
                case ' ':
                    this-> tempMap[i][j] = SPACE_ID;
                    break;
                default:
                    this-> tempMap[i][j] = WALL_ID;
                    break;
            }
        }
    }
}

void Moving::writeTemporaryMapToFile() {

    std::ofstream file(TEMP_MAP_FILE, std::ios::out);

    for(int i = 0; i < this-> engine->getMapXY()[0]; i++) {
        for(int j = 0; j < this-> engine->getMapXY()[1]; j++) {
            file << this-> tempMap[i][j];
        }
        file << std::endl;
    }

    file.close();
}
