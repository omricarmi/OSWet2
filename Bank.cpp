//
// Created by compm on 01/12/17.
//

#include "Bank.h"
#include "common.h"
#include "PrintSafe.h"
#include <unistd.h>
#include <pthread.h>

/***** GLOBALS *****/
//list of bank accounts
std::map<AccountId ,Account&> accounts;
//account of the bank itself
//accountId of bank is irrelevant
Account bankAccount(0,0,0);


void chargeTaxWrapper();

void freeAccounts() {
    //iterate over all accounts and free their allocation
    for (auto& item : accounts) {
        Account &account = item.second;
        //TODO does delete ref delete the origin?
        delete &account;
    }
}

void *bankThreadWrapper(void *bankThreadData) {
    // run until both atms and status are finished
    while(!isATMsFinished || !isStatusFinished){
        //3 sec sleep
        sleep(3);
        //charge taxes from all non VIP accounts
        chargeTaxWrapper();
    }
    //free global accounts
    freeAccounts();
    //TODO check why delete bank dont work
//    delete &bankAccount;
    return NULL;
}

Account &getBankAccount() {
    return bankAccount;
}

void chargeTaxWrapper() {
    srand (time(NULL));
    int percents = rand() % 3 + 2;
    //iterate over all accounts and charge non VIP ones
    for (auto& item : accounts) {
        Account &account = item.second;
        int amountCharged = account.chargeTax(bankAccount,percents);
        //in case vip account
        if(amountCharged != -1){
            std::ostringstream stringStream;
            //Example: Bank: commissions of <#> % were charged, the bank gained <#> $ from account <acc id>
            stringStream << LOG_TAX(percents,amountCharged,account.getId()) << endl;
            string msg = stringStream.str();
            logSafe(msg);
        }
    }
}

