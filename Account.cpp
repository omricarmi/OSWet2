//
// Created by compm on 01/12/17.
//

#include "Account.h"

int Account::getId() const {
    return mId;
}

bool Account::verifyPassword(int password) const {
    return mPassword==password;
}

int Account::getBalance() {
    enterRead();
    int balance = mBalance;
    leaveRead();
    return balance;
}

bool Account::isVIP(){
    enterRead();
    bool isVIP = mIsVIP;
    leaveRead();
    return isVIP;

}

void Account::setVIP(bool isVIP) {
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

int Account::draw(int drawAmount) {
    enterWrite();
    int newBalance = -1;
    if(drawAmount <= mBalance){
        mBalance -= drawAmount;
        newBalance = mBalance;
    }
    leaveWrite();
    return newBalance;
}

int Account::deposit(int depositAmount) {
    //TODO can be a case when deposit A but before log A make another deposit B then log B and then log A, is it OK ?
    //TODO check if need to verify positive deposit
    enterWrite();
    mBalance += depositAmount;
    int newBalance = mBalance;
    leaveWrite();
    return newBalance;
}

int Account::transfer(int transferAmount, Account toAccount) {
    Account& fromAccount = *this;
    if(fromAccount.mId == toAccount.mId){
        return -2; // for same id
    }
    //lock order by id to prevent dead-lock
    if(fromAccount.mId > toAccount.mId) {
        fromAccount.enterWrite();
        toAccount.enterWrite();
    }else{
        toAccount.enterWrite();
        fromAccount.enterWrite();
    }
    int success = -1; //for not enough money
    //transfer money if there is enough
    if(fromAccount.mBalance >= transferAmount){
        //take from one account
        fromAccount.mBalance -= transferAmount;
        //put in the other one
        toAccount.mBalance += transferAmount;
        success = 1;
    }
    //TODO make sure the order is non relevant on unlock
    fromAccount.leaveWrite();
    toAccount.leaveWrite();
    return success;
}
/*
//another option as a friend func.
int transferMoney(int transferAmount, Account fromAccount, Account toAccount) {
    //lock order by id to prevent dead-lock
    if(fromAccount.mId > toAccount.mId) {
        fromAccount.enterWrite();
        toAccount.enterWrite();
    }else{
        toAccount.enterWrite();
        fromAccount.enterWrite();
    }
    int success = -1;
    //transfer money if there is enough
    if(fromAccount.mBalance >= transferAmount){
        //take from one account
        fromAccount.mBalance -= transferAmount;
        //put in the other one
        toAccount.mBalance += transferAmount;
        success = 1;
    }
    //TODO make sure the order is non relevant on unlock
    fromAccount.leaveWrite();
    toAccount.leaveWrite();
    return success;
}
*/
