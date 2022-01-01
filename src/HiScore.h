#ifndef __HISCORE_H_
#define __HISCORE_H_

#include <memory>
#include <iostream>

#define NUMBER_SCORES 5
#define NUMBER_OF_LETTERS 10

class HiScore {

    private:
        char name[NUMBER_SCORES][NUMBER_OF_LETTERS]; // 5
        int score[NUMBER_SCORES]; // 5

    public:
        HiScore();

        // Getters
        char* getName(int i);
        int getScore(int i);

        // Overloading operators
        HiScore & operator<<(const char *);
        HiScore & operator<<(int);

        // Score sorting
        void updateScores(const HiScore &);
        /* void sortScores(); */ // Unessessary

        /* Writing and reading files */
        void writeToFile(const char *);
        void readFromFile(const char *);

};

struct UserNameScorePair {
        char name[NUMBER_SCORES];
        int score;
};

#endif // __HISCORE_H_
