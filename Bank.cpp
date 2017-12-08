//
// Created by compm on 01/12/17.
//

#include "Bank.h"

/***** GLOBALS *****/
//TODO not sure if accounts + bankAccounts need to be globals maybe just main scope
//list of bank accounts
std::map<int,Account&> accounts;
//account of the bank itself
Account bankAccount(0,123456,0); //TODO verify how to init bank account


void *bankThreadWrapper(void *bankThreadData) {
    return nullptr;
}
