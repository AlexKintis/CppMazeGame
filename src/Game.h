#ifndef __GAME_H_
#define __GAME_H_

#include <iostream>
#include <memory>

#include "HiScore.h"
#include "Moving.h"
#include "Engine.h"
#include "Potter.h"
#include "Gnome.h"
#include "Traal.h"
#include "Jewel.h"

#define ARTWORK_PATH "resources/asciiArt.txt"
#define ARTWORK_2_PATH "resources/asciiArt2.txt"
#define ARTWORK_DIED_PATH "resources/asciiArtDoomed.txt"
#define ARTWORK_SUCCESS_PATH "resources/asciiArtSuccess.txt"

#define SHOW_MAP 1
#define SHOW_ENTITIES 1
#define HOSTILES_CAN_CHASE 1
#define PLAY_GAME 1

class Game {

    private:
        std::shared_ptr<Potter> potterPtr;
        std::shared_ptr<Gnome> gnomePtr;
        std::shared_ptr<Traal> traalPtr;
        std::shared_ptr<Engine> enginePtr;
        std::unique_ptr<char> hiScoreFile;
        HiScore score;

    public:
        /* Constructors */
        Game(const char *); // MapX,Y
        Game(const char *, const char *); // MapX,Y

        /* Methods */
        std::vector<std::vector<char>> loadArtWork(const char *);
        void start();
        void initializeDiamonts();
        void moveEntitiesAtMap();

        /* Key Functions */
        bool keyFunctions(int);
};

#endif // __GAME_H_
