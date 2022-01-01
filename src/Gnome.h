#ifndef __GNOME_H_
#define __GNOME_H_

#include "Moving.h"
#include "Colors.h"

#include <math.h>
#include <algorithm>
#include <memory>

class Gnome : public Moving {

    private:
        bool potterEaten;

    public:
        Gnome(Engine *);

        // Eating
        bool canEat(int, int);
        bool canMove(int, int);
        bool isPotterEaten();

        bool canMoveInTempMap(int, int);
        void calculateDistance(int, int);

        void move(int, int);
};

#endif // __GNOME_H_
