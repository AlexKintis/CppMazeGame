#ifndef __POTTER_H_
#define __POTTER_H_

#include "Moving.h"
#include "Colors.h"

class Potter : public Moving {

    private:
        bool alive;
        int eaten;
        char favouriteFood;
        bool isAbleToMoveThere;

    public:
        Potter(Engine *);

        bool canMove(int, int);

        // Living
        bool isAlive();
        void resurrection();
        void die();

        // Getters
        int getEatenDiamonds();
        bool getIsAbleToMoveThere();

        // Eating
        bool canEat(int, int);
};

#endif // __POTTER_H_
