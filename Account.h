//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_ACCOUNT_H
#define OSWET2_ACCOUNT_H

#include <pthread.h>
#include <array>
#include <iostream>
#include <map>
#include "common.h"
typedef int AccountId;

using namespace std;

class TransferData{
private:
public:
    TransferData(int status, int fromNewBalance, int toNewBalance) : status(status),
                                                                                   fromNewBalance(fromNewBalance),
                                                                                   toNewBalance(toNewBalance) {}
    void init(int status, int fromNewBalance, int toNewBalance){
        this->status = status;
        this->fromNewBalance = fromNewBalance;
        this->toNewBalance = toNewBalance;
    }
    int status = -1;
    int fromNewBalance = -1;
    int toNewBalance = -1;
};

class Account {

private:

    //data
    const AccountId mId;
    const int mPassword; //TODO verify 4 digits exactly
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

    Account(int id, int password, int initialAmount) : mId(id), mPassword(password), mBalance(initialAmount), mIsVIP(false) {

        readCount = 0;
        //TODO verify if init mutex make it unlock by default
        int initReadMutexCheck = pthread_mutex_init(&readMutex,NULL);
        int initWriteMutexCheck = pthread_mutex_init(&writeMutex,NULL);
        if (initReadMutexCheck || initWriteMutexCheck)

        {
            string FailedMutex = (initReadMutexCheck != 0) ? "Reader Mutex" : "Writer Mutex";
            cerr << "pthread_mutex_init failed:" << FailedMutex << endl;
            //TODO verify that exit() allowed
            exit(-1);
        }
    }

    ~Account(){
        //TODO verify how to destroy mutex correctly
//        enterRead();
//        leaveRead();
//        enterWrite();
//        leaveWrite();
        if (pthread_mutex_destroy(&readMutex) != 0) {
            cerr << "pthread_mutex_destroy failed: read Mutex." << endl;
            //TODO verify that exit() allowed
            exit(-1);
        }
        if (pthread_mutex_destroy(&writeMutex) != 0) {
            cerr << "pthread_mutex_destroy failed: write Mutex." << endl;
            //TODO verify that exit() allowed
            exit(-1);
        }
    }

    int getId() const;
    bool verifyPassword(int password) const;
    void getBalance(int atmId);
    bool isVIP();
    void setVIP(bool isVIP);
    int draw(int drawAmount, int i);
    int deposit(int depositAmount, int i);
    int chargeTax(Account &bankAccount, double taxPrecents);
    TransferData transfer(int transferAmount, Account &toAccount, int atmId);
    friend string getAccountsStatus(Account& bankAccount);
};

string getAccountsStatus(Account& bankAccount);


#endif //OSWET2_ACCOUNT_H
