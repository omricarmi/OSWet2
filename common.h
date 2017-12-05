//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_CONSTS_H
#define OSWET2_CONSTS_H

#include <string>
#include <iostream>

using namespace std;

#define IS_DEBUG 1

/** Predfined Messages **/
const string FAILED_RUN_ATTEMPT = "illegal arguments\n";

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
#endif //OSWET2_CONSTS_H
