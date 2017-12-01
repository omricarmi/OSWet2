//
// Created by compm on 01/12/17.
//

#include "Account.h"

int Account::getId() const {
    return mId;
}

int Account::getPassword() const {
    return mPassword;
}

int Account::getBalance() {
    enterRead();
    int balance = mBalance;
    leaveRead();
    return balance;
}

void Account::setBalance(int balance) {
    enterWrite();
    mBalance = balance;
    leaveWrite();
}

bool Account::isVIP(){
    enterRead();
    bool isVIP = mIsVIP;
    leaveRead();
    return isVIP;

}

void Account::setIsVIP(bool isVIP) {
    enterWrite();
    mIsVIP = isVIP;
    leaveWrite();
}

void Account::enterRead(){
    pthread_mutex_lock(&readMutex);
    readCount++;
    if(readCount==1){
        pthread_mutex_lock(&writeMutex);
    }
    pthread_mutex_unlock(&readMutex);
}

void Account::leaveRead(){
    pthread_mutex_lock(&readMutex);
    readCount--;
    if(readCount==0){
        pthread_mutex_unlock(&writeMutex);
    }
    pthread_mutex_unlock(&readMutex);
}

void Account::enterWrite(){
    pthread_mutex_lock(&writeMutex);
}

void Account::leaveWrite() {
    pthread_mutex_unlock(&writeMutex);
}