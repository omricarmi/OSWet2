//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_BANK_H
#define OSWET2_BANK_H


#include "Account.h"

typedef struct _BankThreadData{
    Account* pBankAccount;
} BankThreadData;

void* bankThreadWrapper(void* bankThreadData);

class Bank {
private:
    Account bankAccount;
public:

};


#endif //OSWET2_BANK_H
