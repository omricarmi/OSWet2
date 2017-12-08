//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_BANK_H
#define OSWET2_BANK_H


#include "Account.h"

/***** GLOBALS *****/
//TODO not sure if accounts + bankAccounts need to be globals maybe just main scope
//list of bank accounts
extern std::map<int,Account&> accounts;


typedef struct _BankThreadData{
    Account* pBankAccount;
} BankThreadData;

void* bankThreadWrapper(void* bankThreadData);

class Bank {
private:
    //TODO decide what better?
    // origin bank account here with ref in the main
    //      or
    // origin bank account in the main with ref here
    Account& bankAccount;
public:

};


#endif //OSWET2_BANK_H
