#ifndef __TRAAL_H_
#define __TRAAL_H_

#include "Moving.h"
#include "Colors.h"

#include <math.h>
#include <algorithm>

class Traal : public Moving {

    private:
        bool potterEaten;


    public:
        Traal(Engine *);

        // Eating
        bool canEat(int, int);
        bool canMove(int, int);
        bool isPotterEaten();

        bool canMoveInTempMap(int, int);
        void calculateDistance(int, int);

        void move(int, int);
};

#endif // __TRAAL_H_
