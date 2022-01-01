#include "Potter.h"

/* Constructors */
Potter::Potter(Engine *engine) : Moving(engine) {
    this-> entityChar = 'M';
    this-> eatingHabits = 'D';
    this-> favouriteFood = 'F';
    this-> color = POTTER_PAIR;
    this-> eaten = 0;
}

bool Potter::canMove(int Y, int X) {

    int maxY = this-> engine ->getMapXY()[0];
    int maxX = this-> engine ->getMapXY()[1];

    int ch = this->engine->getCharFromMap(Y, X);

    this-> isAbleToMoveThere = (ch == ' ' || ch == 'G' || ch == 'T') && (Y > 0 && Y < maxY) && (X > 0 && X < maxX);

    return this-> isAbleToMoveThere;
}

bool Potter::isAlive() { return this-> alive; }

void Potter::resurrection() { this-> alive = true; }

void Potter::die() { this-> alive = false; }

// Getters
int Potter::getEatenDiamonds() { return this-> eaten; }

bool Potter::getIsAbleToMoveThere() { return this-> isAbleToMoveThere; }

// Moving

bool Potter::canEat(int Y, int X) {

    bool eat = false;
    int ch = this->engine->getCharFromMap(Y, X);

    eat = (ch == this->eatingHabits) || (ch == this-> favouriteFood);

    if(eat) {
        if(!(ch == this-> favouriteFood)) {
            this-> eaten += 10;
        } else {
            this-> eaten += 100;
        }
    }

    return eat;
}
