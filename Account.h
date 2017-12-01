//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_ACCOUNT_H
#define OSWET2_ACCOUNT_H


#include <pthread.h>
#include <array>
#include <iostream>

using namespace std;
class Account {

private:

    //data
    int mId;
    int mPassword;
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

public:

    Account(int id, int password, int balance) : mId(id), mPassword(password), mBalance(balance) {

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

    int getId() const;

    int getPassword() const;

    int getBalance();

    void setBalance(int balance);

    bool isVIP();

    void setIsVIP(bool isVIP);


};




#endif //OSWET2_ACCOUNT_H
