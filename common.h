//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_CONSTS_H
#define OSWET2_CONSTS_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "PrintSafe.h"
#include <unistd.h>

using namespace std;

extern bool isATMsFinished;
extern bool isStatusFinished;

#define IS_DEBUG 1

/** Predfined Messages **/
#define ERR_FAILED_RUN_ATTEMPT \
    "illegal arguments"
#define LOG_ACCOUNT_NOT_EXISTS(atmId,accountId) \
    "Error " << atmId << ": Your transaction failed – account id " << accountId << " does not exist"
#define LOG_TAX(taxPercent,amountCharged,accountId) \
    "Bank: commissions of " << taxPercent << " % were charged, the bank gained " << amountCharged << " $ from account " << accountId
#define LOG_WRONG_PASSWORD(atmId,accountId) \
    "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect"
#define LOG_ACCOUNT_ALREADY_EXISTS(atmId) \
    "Error " << atmId << ": Your transaction failed – account with the same id exists"
#define LOG_ACCOUNT_CREATED(atmId,accountId,password,initialAmount) \
    atmId << ": New account id is " << accountId << " with password " << password << " and initial balance " << initialAmount
#define LOG_DEPOSIT(atmId,accountId,newBalance,amount) \
    atmId << ": Account " << accountId << " new balance is " << newBalance << " after " << amount << " $ was deposited"
#define LOG_WITHDRAW(atmId,accountId,withdrawResult,amount) \
    atmId << ": Account " << accountId << " new balance is " << withdrawResult << " after " << amount << " $ was withdrew"
#define LOG_LOW_BALANCE(atmId,accountId,amount) \
    "Error " << atmId << ": Your transaction failed – account id " << accountId << " balance is lower than " << amount
#define LOG_BALANCE(atmId,accountId,balance) \
    atmId << ": Account " << accountId << " balance is " << balance
#define LOG_TRANSFER(atmId,transferAmount,accountId,targetAccountId,data) \
    atmId << ": Transfer " << transferAmount << " from account " << accountId << " to account " \
    << targetAccountId << " new account balance is " << data.fromNewBalance \
    << " new target account balance is "<< data.toNewBalance

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
