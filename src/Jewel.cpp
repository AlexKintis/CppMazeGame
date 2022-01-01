#include "Jewel.h"

// Constructor
Jewel::Jewel(Engine *engine, bool last) {

    if(last) {
        this-> entity = 'F';
    } else {
        this-> entity = 'D';
    }

    int *tempYX = engine-> spawnPlaceRandomizer();
    this-> y = tempYX[0];
    this-> x = tempYX[1];
}

/* Getters */
char Jewel::getEntity() { return this-> entity; }

int Jewel::getY() { return this-> y; };

int Jewel::getX() { return this-> x; };
