//
// Created by compm on 01/12/17.
//

#include <sstream>
#include <cmath>
#include "Account.h"
#include "Bank.h"

int Account::getId() const {
    return mId;
}

bool Account::verifyPassword(int password) const {
    return mPassword==password;
}

int Account::getBalance() {
    enterRead();
    //make 1 sec delay
    sleep(1);
    int balance = mBalance;
    leaveRead();
    return balance;
}

//TODO safe delete this function
//bool Account::isVIP(){
//    enterRead();
//    bool isVIP = mIsVIP;
//    leaveRead();
//    return isVIP;
//
//}

void Account::setVIP(bool isVIP) {
    enterWrite();
    //make 1 sec delay
    sleep(1);
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
    //make 1 sec delay
    sleep(1);
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
    //make 1 sec delay
    sleep(1);
    mBalance += depositAmount;
    int newBalance = mBalance;
    leaveWrite();
    return newBalance;
}

TransferData Account::transfer(int transferAmount, Account& toAccount) {
    Account& fromAccount = *this;
    if(fromAccount.mId == toAccount.mId){
        return TransferData(-2,-1,-1); // for same id error
    }
    //lock order by id to prevent dead-lock
    if(fromAccount.mId < toAccount.mId) {
        fromAccount.enterWrite();
        toAccount.enterWrite();
    }else{
        toAccount.enterWrite();
        fromAccount.enterWrite();
    }
    //make 1 sec delay
    sleep(1);
    TransferData data(-1,-1,-1);
    //transfer money if there is enough
    if(fromAccount.mBalance >= transferAmount){
        //take from one account
        fromAccount.mBalance -= transferAmount;
        //put in the other one
        toAccount.mBalance += transferAmount;
        data.init(1,fromAccount.mBalance,toAccount.mBalance);
    }
    //TODO make sure the order is non relevant on unlock
    if(fromAccount.mId < toAccount.mId){
        toAccount.leaveWrite();
        fromAccount.leaveWrite();
    } else{
        fromAccount.leaveWrite();
        toAccount.leaveWrite();
    }
    return data;
}

string getAccountsStatus(Account& bankAccount) {
    //lock all accounts include bank account
    //TODO make sure locked in the same order every time to prevent deadlock
    bankAccount.enterRead();
    for (auto& item : accounts){
        Account& account = item.second;
        account.enterRead();
    }

    //generate status of the bank system
    string status = "Current Bank Status\n";
    for (auto& item : accounts){
        Account& account = item.second;
        status += account.getStatus();
    }
    //Example: "The Bank has 1500 $\n"
    status += string("The Bank has ") + to_string(bankAccount.mBalance) + string(" $\n");

    //unlock all accounts include bank account
    //TODO check about unlock reverse order
    bankAccount.leaveRead();
    for (auto& item : accounts){
        Account& account = item.second;
        account.leaveRead();
    }

    return status;
}

string Account::getStatus() {
//   Example: "Account 123: Balance – 12  $ , Account Password – 1234\n"
    //TODO handle fixed width for balance , check what the correct width
    std::ostringstream stringStream;
    stringStream << "Account " << mId << ": Balance – " << mBalance << "  $ , Account Password – " << mPassword << endl;
    string status = stringStream.str();
    return status;
}

int Account::chargeTax(Account &bankAccount, double taxPrecents) {
    Account& account = *this;
    //always lock bank first to prevent dead-lock
    bankAccount.enterWrite();
    account.enterWrite();
    //if vip return -1 and finish
    if(account.mIsVIP){
        return -1;
    }
    //calc tax in money round to nearest integer value
    int taxAmount = (int) round( (taxPrecents/100.0) * ((double)account.mBalance) );
    account.mBalance -= taxAmount;
    bankAccount.mBalance += taxAmount;
    //TODO make sure the order is non relevant on unlock
    bankAccount.leaveWrite();
    account.leaveWrite();
    return taxAmount;
}


/*
//another option as a friend func.
int transferMoney(int transferAmount, Account& fromAccount, Account& toAccount) {
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

