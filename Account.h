//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_ACCOUNT_H
#define OSWET2_ACCOUNT_H

#include <pthread.h>
#include <array>
#include <iostream>
#include <map>

using namespace std;

class Account {

private:

    //data
    const int mId;
    const int mPassword;
    int mBalance;
    bool mIsVIP;

    //locks for read/write sync
    pthread_mutex_t readMutex;
    pthread_mutex_t writeMutex;
    int readCount;
    void enterRead();
    void leaveRead();
    void enterWrite();
    void leaveWrite();
    string getStatus();

public:

    Account(int id, int password) : mId(id), mPassword(password), mBalance(0), mIsVIP(false) {

        readCount = 0;
        //TODO verify if init mutex make it unlock by default
        int initReadMutexCheck = pthread_mutex_init(&readMutex,NULL);
        int initWriteMutexCheck = pthread_mutex_init(&writeMutex,NULL);
        if (initReadMutexCheck || initWriteMutexCheck)

        {
            string FailedMutex = (initReadMutexCheck != 0) ? "Reader Mutex" : "Writer Mutex";
            cerr << "pthread_mutex_init failed:" << FailedMutex << endl;
            //TODO verift that exit() allowed
            exit(-1);
        }
    }

    //TODO make all ATM orders with sleep(1)
    int getId() const;
    bool verifyPassword(int password) const;
    int getBalance();
    bool isVIP();
    void setVIP(bool isVIP);
    int draw(int drawAmount);
    int deposit(int depositAmount);
    int transfer(int transferAmount, Account toAccount);
//    friend int transferMoney(int transferAmount,Account fromAccount,Account toAccount);
    friend string getAccountsStatus(std::map<int,Account> accounts,Account bankAccount);
};

string getAccountsStatus(std::map<int,Account> accounts,Account bankAccount);
int transferMoney(int transferAmount,Account fromAccount,Account toAccount);


#endif //OSWET2_ACCOUNT_H
