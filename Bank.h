//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_BANK_H
#define OSWET2_BANK_H


#include "Account.h"
#include "common.h"

/***** GLOBALS *****/
//list of bank accounts
extern std::map<AccountId ,Account&> accounts;

typedef struct _BankThreadData{
    Account* pBankAccount;
} BankThreadData;

void* bankThreadWrapper(void* bankThreadData);

Account& getBankAccount();

#endif //OSWET2_BANK_H
