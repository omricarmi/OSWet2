//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_CONSTS_H
#define OSWET2_CONSTS_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;


#define IS_DEBUG 1

/** Predfined Messages **/
const string FAILED_RUN_ATTEMPT = "illegal arguments\n";
//TODO make below macro to all messages
#define ACCOUNT_NOT_EXISTS(atmId,accountId) "Error " << atmId << ": Your transaction failed – account id " << accountId << " does not exist"

class Log{
public:
    static bool isDebug;
    void operator<< (const string& msg){
        if(isDebug) cout << "debug :\n" << msg << endl;
    }
};
bool Log::isDebug = IS_DEBUG;

void logd(string msg){
    if(IS_DEBUG) {
        std::cout << "debug: " << msg << endl;
    }
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
#endif //OSWET2_CONSTS_H
