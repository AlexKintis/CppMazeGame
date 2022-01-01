#include "Gnome.h"
#include "Engine.h"
#include "Moving.h"
#include <algorithm>
#include <limits.h>
#include <curses.h>
#include <fstream>
#include <memory>

#include "Potter.h"

Gnome::Gnome(Engine *engine) : Moving(engine) {
    this-> entityChar = 'G';
    this-> color = HOSTILE_PAIR;
    this-> eatingHabits = 'M';

    this-> potterEaten = false;

    int *mapYX = this-> engine -> getMapXY();

    this-> tempMap = std::unique_ptr<std::unique_ptr<int[]>[]>(new std::unique_ptr<int[]>[mapYX[0]]);

    for(unsigned int i = 0; i < mapYX[0]; i++) {
        this-> tempMap[i] = std::unique_ptr<int[]>(new int[mapYX[1]]);
    }
}

bool Gnome::canEat(int Y, int X) {

    bool flag;

    if(this->engine->getCharFromMap(Y - 1, X) == this->eatingHabits)
        flag = true;
    if(this->engine->getCharFromMap(Y + 1, X) == this->eatingHabits)
        flag = true;
    if(this->engine->getCharFromMap(Y, X - 1) == this->eatingHabits)
        flag = true;
    if(this->engine->getCharFromMap(Y, X + 1) == this->eatingHabits)
        flag = true;

    return flag;
}

bool Gnome::canMove(int Y, int X) {

    int maxY = this-> engine ->getMapXY()[0];
    int maxX = this-> engine ->getMapXY()[1];

    int ch = this->engine->getCharFromMap(Y, X);

    return (ch == ' ' || ch == 'D') && (Y > 0 && Y < maxY) && (X > 0 && X < maxX);
}

bool Gnome::isPotterEaten() { return this-> potterEaten; }

bool Gnome::canMoveInTempMap(int Y, int X) {

    int item = this-> tempMap[Y][X];

    bool flag = item != WALL_ID && item != GNOME_ID && item != POTTER_ID && item != TRAAL_ID;

    return flag;
}

void Gnome::calculateDistance(int Y, int X) {

    Engine e = *(this-> engine);

    unique2D &map = this-> tempMap;
    int mapY = e.getMapXY()[0];
    int mapX = e.getMapXY()[1];

    if(this-> canMoveInTempMap(Y-1, X)){
        map[Y-1][X] = 1;
    }

    if(this-> canMoveInTempMap(Y+1, X)){
        map[Y+1][X] = 1;
    }

    if(this-> canMoveInTempMap(Y, X-1)){
        map[Y][X-1] = 1;
    }
    if(this-> canMoveInTempMap(Y, X+1)) {
        map[Y][X+1] = 1;
    }

    int step = 1;
    bool selfFound = false;

    do {

        for(int y = 0; y < mapY; y++) {
            for(int x = 0; x < mapX; x++) {

                if(map[y][x] == step) {

                    if(map[y - 1][x] == GNOME_ID) {
                        selfFound = true;
                    } else if(this-> canMoveInTempMap(y - 1, x)) {
                        map[y - 1][x] = step + 1;
                    }

                    if (map[y + 1][x] == GNOME_ID) {
                        selfFound = true;
                    }else if (this-> canMoveInTempMap(y + 1, x)) {
                        map[y + 1][x] = step + 1;
                    }

                    if (map[y][x + 1] == GNOME_ID) {
                        selfFound = true;
                    } else if (this-> canMoveInTempMap(y, x + 1)) {
                        map[y][x + 1] = step + 1;
                    }

                    if (map[y][x - 1] == GNOME_ID) {
                        selfFound = true;
                    } else if (this-> canMoveInTempMap(y, x - 1)) {
                        map[y][x - 1] = step + 1;
                    }

                }

            }
        }

        step++;

        if(step == (mapX * mapY / 10) - 1) {
            break;
        }

    } while(!selfFound);

}

void Gnome::move(int Y, int X) {

    unique2D &map = this-> tempMap;

    this-> initiateTemporaryMap();
    this-> calculateDistance(Y, X);

    /* Uncomment this to see file paths
     * and use this command in project root to see them
     *
     * watch -n 0.1 "gawk '{gsub(-8,\"*\"); gsub(0,\" \"); print}' RS='^$'" TEMP_MAP.txt
     *
     */
    // this-> writeTemporaryMapToFile();

    std::vector<int> directions;

    if(map[this->currY - 1][this->currX] > 0) {
        directions.push_back(map[this->currY - 1][this->currX]); // up
    } else {
        directions.push_back(INT_MAX);
    }

    if(map[this->currY + 1][this->currX] > 0) {
        directions.push_back(map[this->currY + 1][this->currX]); // down
    } else {
        directions.push_back(INT_MAX);
    }

    if(map[this->currY][this->currX - 1] > 0) {
        directions.push_back(map[this->currY][this->currX - 1]); // left
    } else {
        directions.push_back(INT_MAX);
    }

    if(map[this->currY][this->currX + 1] > 0) {
        directions.push_back(map[this->currY][this->currX + 1]); // right
    } else {
        directions.push_back(INT_MAX);
    }

    std::vector<int>::iterator min = std::min_element(directions.begin(), directions.end());

    int direction = std::distance(directions.begin(), min);

    switch(direction) {
        case 0:
            this-> moveUp();
            if(this-> canEat(currY, currX)) {
                this-> potterEaten = true;
            }
            break;
        case 1:
            this-> moveDown();
            if(this-> canEat(currY, currX)) {
                this-> potterEaten = true;
            }
            break;
        case 2:
            this-> moveLeft();
            if(this-> canEat(currY, currX)) {
                this-> potterEaten = true;
            }
            break;
        case 3:
            this-> moveRight();
            if(this-> canEat(currY, currX)) {
                this-> potterEaten = true;
            }
            break;
    }

}
