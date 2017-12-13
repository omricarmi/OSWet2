//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_CONSTS_H
#define OSWET2_CONSTS_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>

using namespace std;


#define IS_DEBUG 1

/** Predfined Messages **/
const string FAILED_RUN_ATTEMPT = "illegal arguments\n";
//TODO make below macro to all messages
#define ACCOUNT_NOT_EXISTS(atmId,accountId) "Error " << atmId << ": Your transaction failed – account id " << accountId << " does not exist"
#define TAX_LOG(taxPercent,amountCharged,accountId) "Bank: commissions of " << taxPercent << " % were charged, the bank gained " << amountCharged << " $ from account " << accountId

//class Log{
//public:
//    static bool isDebug;
//    void operator<< (const string& msg){
//        if(isDebug) cout << "debug :\n" << msg << endl;
//    }
//};
//bool Log::isDebug = IS_DEBUG;

void logd(string msg);

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim);
#endif //OSWET2_CONSTS_H
