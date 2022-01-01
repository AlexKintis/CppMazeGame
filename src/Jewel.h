#ifndef __JEWEL_H_
#define __JEWEL_H_

#include "Moving.h"
#include "Engine.h"

class Jewel {

    private:
        char entity;
        int y;
        int x;
    public:
        Jewel(Engine *, bool);

        // Getters
        char getEntity();
        int getY();
        int getX();

};

#endif // __JEWEL_H_
