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
Account bankAccount(0,0,0); //TODO verify how to init bank account


void chargeTaxWrapper();
void *bankThreadWrapper(void *bankThreadData) {
    bool isContinue = true;
    while(isContinue){
        //3 sec sleep
        sleep(3);
        //charge taxes from all non VIP accounts
        chargeTaxWrapper();
    }
}

Account &getBankAccount() {
    return bankAccount;
}

void chargeTaxWrapper() {
    //TODO is tax is integer precent 2% 3% 4% ???????????????????????????????
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
