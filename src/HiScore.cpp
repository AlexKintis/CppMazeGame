#include "HiScore.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>
#include <vector>

HiScore::HiScore() {

    for(unsigned int i = 0; i < NUMBER_SCORES; i++) {
        strcpy(this-> name[i], "NULL");
    }

    memset(score, 0, sizeof(score));
}


char* HiScore::getName(int index) { return this-> name[index]; }

int HiScore::getScore(int index) { return this-> score[index]; }

HiScore & HiScore::operator<<(const char *name) {

    strcpy(this-> name[0], name);

    return *this;
}

HiScore & HiScore::operator<<(int in_score) {

    this-> score[0] = in_score;

    return *this;
}

void HiScore::updateScores(const HiScore& user_score) {

    std::vector<UserNameScorePair> vec;
    UserNameScorePair user;

    strcpy(user.name, user_score.name[0]);
    user.score = user_score.score[0];

    // Parse scores to UserNameScorePair Struct
    for(unsigned int i = 0; i < NUMBER_SCORES; i++) {

        UserNameScorePair pair;

        strcpy(pair.name, this-> name[i]);
        pair.score = this-> score[i];

        vec.push_back(pair);
    }

    // Find the right spot to insert user's score inside scores (struct)
    for(unsigned int i = 0; i < vec.size(); i++) {
        if(user.score >= vec[i].score) {
            vec.insert(vec.begin() + i, user);
            break;
        }
    }

    // Parse back the scores after the insertion of user's
    // score

    std::vector<UserNameScorePair>::iterator iter;

    for(iter = vec.begin(); iter != vec.begin() + NUMBER_SCORES; iter++) {
        strcpy(this-> name[iter - vec.begin()], iter -> name);
        this-> score[iter - vec.begin()] = iter -> score;
    }

}

/*
 * Unessessary Function
 *
void HiScore::sortScores() {

    std::vector<std::pair<int, std::string>> scores;

    for(unsigned int i = 0; i < NUMBER_SCORES; i++) {
        scores.push_back(std::pair<int, std::string>(score[i], std::string(name[i])));
    }

    // using lamba to sort vector using <algorithm>
    std::sort(scores.begin(), scores.end(),
              [](const std::pair<int, std::string> &l, const std::pair<int, std::string> &r) {
                  return l.first > r.first;
              });

    std::vector<std::pair<int, std::string>>::iterator it;

    for(it = scores.begin(); it != scores.end(); it++) {
        int index = std::distance(scores.begin(), it);
        this-> score[index] = it->first;
        strcpy(name[index], it->second.c_str());
    }

}
*/

// Writing to binary file
void HiScore::writeToFile(const char *fileName) {

    std::ofstream outputFile(fileName, std::ios::out | std::ios::binary);

    HiScore &self = *this;

    for(int i = 0; i < 5; i++) {
        outputFile.write((char*)&self.name[i], sizeof(self.name[i]));
        outputFile.write((char*)&self.score[i], sizeof(self.score[i]));
    }

    outputFile.close();
}

// Reading from binary file
void HiScore::readFromFile(const char *fileName) {

    std::ifstream inputFile(fileName, std::ios::out | std::ios::binary);

    HiScore &self = *this;

    for(unsigned int i = 0; i < 5; i++) {
        inputFile.read((char*)&self.name[i], sizeof(self.name[i]));
        inputFile.read((char*)&self.score[i], sizeof(self.score[i]));
    }

    inputFile.close();

}
