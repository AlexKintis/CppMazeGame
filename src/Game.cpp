#include "Game.h"
#include "Moving.h"

#include <curses.h>
#include <fstream>
#include <memory>
#include <cstring>

#include "Colors.h"

/* Constructors */
Game::Game(const char *mapFilePath) {

    this-> enginePtr = std::make_shared<Engine>(Engine(mapFilePath));

    // Hiscore file
    this-> hiScoreFile = std::make_unique<char>(strlen("HiScores.bin"));
    strcpy(this-> hiScoreFile.get(), "HiScores.bin");
    this-> score.readFromFile(this-> hiScoreFile.get());
}

Game::Game(const char *mapFilePath, const char * hiScoreFilePath) {

    this-> enginePtr = std::make_shared<Engine>(Engine(mapFilePath));

    // Hiscore file
    this-> hiScoreFile = std::make_unique<char>(strlen(hiScoreFilePath));
    strcpy(this-> hiScoreFile.get(), hiScoreFilePath);
    this-> score.readFromFile(this-> hiScoreFile.get());

}

std::vector<std::vector<char>> Game::loadArtWork(const char *filename) {

    std::ifstream artWorkFile(filename);
    std::string line;
    std::vector<std::vector<char>> artWork;

    try {
        if(!artWorkFile.is_open()) {
            throw "File opening error!";
        }
    } catch(char *msg) {
        std::cout << msg << std::endl;
        return artWork;
    }

    // Read characters and parse them to artWork vector
    std::vector<char> lineVec;
    while(getline (artWorkFile, line)) {
        for(auto ch : line)
            lineVec.push_back(ch);
        artWork.push_back(lineVec);
        lineVec.clear();
    }

    // Remove newLine characters
    for(auto line : artWork) {
        line.pop_back();
    }

    // Getting the max of each line in order to fill it later
    int max = -999;
    for(auto line : artWork) {
        if((int)line.size() > max) {
            max = line.size();
        }
    }

    // Fill the remaining rows with space character
    for(int y = 0; y < artWork.size(); y++) {
        while(artWork[y].size() < max) {
            artWork[y].push_back(' ');
        }
    }

    return artWork;
}

void Game::start() {

    Engine &engine = *(this-> enginePtr);

    engine.initScreen();

    // Drawing art work
    engine.drawArtWork(this-> loadArtWork(ARTWORK_PATH));

#if SHOW_MAP
    engine.drawMap();
#endif

    this-> potterPtr = std::make_shared<Potter>(Potter(&engine));
    this-> gnomePtr = std::make_shared<Gnome>(Gnome(&engine));
    this-> traalPtr = std::make_shared<Traal>(Traal(&engine));

    Potter &potter = *(this-> potterPtr);
    Gnome &gnome = *(this-> gnomePtr);
    Traal &traal = *(this-> traalPtr);

#if SHOW_ENTITIES

    /* Spawn entities */
    engine.spawnEntityAtMap(potter.getEntityChar(), potter.getCurrentY(), potter.getCurrentX(), potter.getColor());
    engine.spawnEntityAtMap(gnome.getEntityChar(), gnome.getCurrentY(), gnome.getCurrentX(), gnome.getColor());
    engine.spawnEntityAtMap(traal.getEntityChar(), traal.getCurrentY(), traal.getCurrentX(), traal.getColor());

    // Initialize Diamonds / Jewels
    this-> initializeDiamonts();

#endif

    int eatenDiamonds = potter.getEatenDiamonds();
    bool finalParchmentSpawned = false;

#if PLAY_GAME

    while(!engine.escKeyPressed() && potter.isAlive() && eatenDiamonds < 200) {

#if SHOW_ENTITIES

        // Moving entities at map
        this-> moveEntitiesAtMap();

#endif

        if(eatenDiamonds == 100 && !finalParchmentSpawned) {
            Jewel jewel(this-> enginePtr.get(), true);
            this-> enginePtr->spawnEntityAtMap(jewel.getEntity(), jewel.getY(), jewel.getX(), DIAMOND_PAIR);
            finalParchmentSpawned = true;
        }

        engine.drawScore(eatenDiamonds);

        engine.captureKeyPress();

        bool acceptedKeyPress = this-> keyFunctions(engine.getKeyCode());
        while(!acceptedKeyPress) {
            engine.captureKeyPress();
            acceptedKeyPress = this-> keyFunctions(engine.getKeyCode());
        }

        eatenDiamonds = potter.getEatenDiamonds();

        if(gnome.isPotterEaten() || traal.isPotterEaten()) {
            potter.die();
        }

        engine.refreshRootWin();
        engine.refreshMapWin();

    }

#endif

    /* Game is finished */
    // Draw Score
    engine.drawScore(eatenDiamonds);
    engine.refreshRootWin();

    // Game is finished so user is asked for name
    engine.clearMapWindow();

    if(eatenDiamonds == 200) {
        engine.drawArtWork(this-> loadArtWork(ARTWORK_SUCCESS_PATH));
    } else {
        engine.drawArtWork(this-> loadArtWork(ARTWORK_DIED_PATH));
    }

    std::string playerName = engine.askForUserName();

    // Writing to scoreboard
    HiScore temp_score;
    temp_score << playerName.c_str() << eatenDiamonds;

    this-> score.updateScores(temp_score);

    engine.displayScores(this-> score);

    engine.destroyScreen();

    this-> score.writeToFile(this-> hiScoreFile.get());

}

void Game::initializeDiamonts() {

    for(unsigned int i = 0; i < 10; i++) {
        Jewel jewel(this-> enginePtr.get(), false);
        this-> enginePtr->spawnEntityAtMap(jewel.getEntity(), jewel.getY(), jewel.getX(), DIAMOND_PAIR);
    }
}

void Game::moveEntitiesAtMap() {

    Engine &engine = *(this-> enginePtr);

    Potter &potter = *(this-> potterPtr);
    Gnome &gnome = *(this-> gnomePtr);
    Traal &traal = *(this-> traalPtr);

    static int gnomeDiamondsFound = 0;
    static int traalDiamondsFound = 0;

    // move Potter
    engine.moveEntityAtMap(potter.getEntityChar(), potter.getPreviousY(),
                           potter.getPreviousX(), potter.getCurrentY(), potter.getCurrentX(), potter.getColor());

#if HOSTILES_CAN_CHASE
    if(potter.getIsAbleToMoveThere()) {
        gnome.move(potter.getCurrentY(), potter.getCurrentX());
        traal.move(potter.getCurrentY(), potter.getCurrentX());
    }
#endif

    if(engine.getCharFromMap(gnome.getCurrentY(), gnome.getCurrentX()) == 'D') {
        gnomeDiamondsFound++;
    }

    if(engine.getCharFromMap(traal.getCurrentY(), traal.getCurrentX()) == 'D') {
        traalDiamondsFound++;
    }

    engine.moveEntityAtMap(gnome.getEntityChar(), gnome.getPreviousY(),
                           gnome.getPreviousX(), gnome.getCurrentY(), gnome.getCurrentX(), gnome.getColor());

    engine.moveEntityAtMap(traal.getEntityChar(), traal.getPreviousY(),
                           traal.getPreviousX(), traal.getCurrentY(), traal.getCurrentX(), traal.getColor());

    if(gnomeDiamondsFound > 0) {
        engine.spawnEntityAtMap('D', gnome.getPreviousY(), gnome.getPreviousX(), DIAMOND_PAIR);
        gnomeDiamondsFound--;
    }

    if(traalDiamondsFound > 0) {
        engine.spawnEntityAtMap('D', traal.getPreviousY(), traal.getPreviousX(), DIAMOND_PAIR);
        traalDiamondsFound--;
    }



}

// this function prints map in as text in terminal -> not Ncurses
bool Game::keyFunctions(int key) {

    Potter &potter = *(this-> potterPtr);

    switch(key) {
        case KEY_UP:
            potter.moveUp();
            return true;
        case KEY_DOWN:
            potter.moveDown();
            return true;
        case KEY_LEFT:
            potter.moveLeft();
            return true;
        case KEY_RIGHT:
            potter.moveRight();
            return true;
        case 32:
            return true;
        case 27:
            return true;
        default:
            return false;
    }
}
