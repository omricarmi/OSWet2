//
// Created by compm on 13/12/17.
//

#include "common.h"

void logd(string msg){
    if(IS_DEBUG) {
        std::cout << "debug: " << msg << endl;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}