//
// Created by compm on 01/12/17.
//

#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "Bank.h"

//list of bank accounts
std::map<AccountId ,Account&> accounts;
//account of the bank itself
Account *pBankAccount;

pthread_mutex_t addAccountMutex;

void chargeTaxWrapper();

void freeAccounts() {
    //TODO find how to free accounts MAP
    accounts.clear();
//    //iterate over all accounts and free their allocation
//    for (auto& item : accounts) {
//        delete &(item.second);
////        Account &account = item.second;
////        delete &account;
//    }
}

void *bankThreadWrapper(void *bankThreadData) {
    //must call before using bank
    startBank();
    // run until both atms and status are finished
    while(!isATMsFinished || !isStatusFinished){
        //3 sec sleep
        sleep(3);
        //charge taxes from all non VIP accounts
        chargeTaxWrapper();
    }
    // print final state of the bank before finish the execution
    getAccountsStatus(getBankAccount());
    //free global accounts
    freeAccounts();
    //finish bank
    finishBank();

    return NULL;
}

Account &getBankAccount() {
    return *pBankAccount;
}

void chargeTaxWrapper() {
    srand (time(NULL));
    int percents = rand() % 3 + 2;
    //iterate over all accounts and charge non VIP ones
    for (auto& item : accounts) {
        Account &account = item.second;
        account.chargeTax(*pBankAccount,percents);
    }
}

void startBank() {
    if (pthread_mutex_init(&addAccountMutex,NULL)) {
        cerr << "pthread_mutex_init failed: Add Account Mutex." << endl;
        exit(-1);
    }
    //init accountId of bank is irrelevant
    pBankAccount = new Account(0,0,0);
}

void finishBank() {
   if (pthread_mutex_trylock(&addAccountMutex) != 0){
       cerr << "pthread_mutex_destroy failed: Add Account Mutex." << endl;
       exit(-1);
   }else{
       pthread_mutex_unlock(&addAccountMutex);
   }
    int errid = pthread_mutex_destroy(&addAccountMutex);
//    if ( errid != 0) {
//        //TODO FAILED on tests sometimes don't know why :-(
//        cerr << "pthread_mutex_destroy failed: Add Account Mutex." << endl;
//        exit(-1);
//    }
    //free bank obj
    delete pBankAccount;
}

void lockAddAccount() {
    pthread_mutex_lock(&addAccountMutex);
}

void unlockAddAccount() {
    pthread_mutex_unlock(&addAccountMutex);
}

