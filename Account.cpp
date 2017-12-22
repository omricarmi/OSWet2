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

void Account::getBalance(int atmId) {
    enterRead();
    //make 1 sec delay
    sleep(1);
    std::ostringstream stringStream;
    //Example: <ATM ID>: Account <id> balance is <bal>
    stringStream << LOG_BALANCE(atmId,mId,mBalance) << endl;
    string msg = stringStream.str();
    logSafe(msg);
    leaveRead();
}

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

int Account::draw(int drawAmount, int atmId) {
    enterWrite();
    //make 1 sec delay
    sleep(1);
    int newBalance = -1;
    if(drawAmount <= mBalance){
        mBalance -= drawAmount;
        newBalance = mBalance;
    }
    // in case the withdraw amount is bigger than the current balance
    if ( newBalance == -1 ){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_LOW_BALANCE(atmId,mId,drawAmount) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }else {
        std::ostringstream stringStream;
        // log for success withdrawal
        //Example:<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew
        stringStream << LOG_WITHDRAW(atmId, mId, newBalance, drawAmount) << endl;
        string msg = stringStream.str();
        logSafe(msg);
    }
    leaveWrite();
    return newBalance;
}

int Account::deposit(int depositAmount, int atmId) {
    enterWrite();
    //make 1 sec delay
    sleep(1);
    mBalance += depositAmount;
    int newBalance = mBalance;
    //log deposit
    std::ostringstream stringStream;
    //Example: <ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited 
    stringStream << LOG_DEPOSIT(atmId,mId,newBalance,depositAmount) << endl;
    string msg = stringStream.str();
    logSafe(msg);
    leaveWrite();
    return newBalance;
}

TransferData Account::transfer(int transferAmount, Account &toAccount, int atmId) {
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

    if(data.status == 1){
        //log success transfer
        std::ostringstream stringStream;
        //Example (is one line): <ATM ID>: Transfer <amount> from account <account> to account <target_account>
        // new account balance is <account_bal> new target account balance is <target_bal>
        stringStream << LOG_TRANSFER(atmId,transferAmount,fromAccount.mId,toAccount.mId,data) << endl;
        string msg = stringStream.str();
        logSafe(msg);
    }else{
        //on transfer failed log it
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>
        stringStream << LOG_LOW_BALANCE(atmId,fromAccount.mId,transferAmount) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);

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

    //clear screen
    printSafe("\033[2J");
    //move cursor to left up corner
    printSafe("\033[1;1H");
    //print status
    printSafe(status);

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
    //be sure you call this after locking the account
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

    //in case not vip account
    std::ostringstream stringStream;
    //Example: Bank: commissions of <#> % were charged, the bank gained <#> $ from account <acc id>
    stringStream << LOG_TAX(taxPrecents,taxAmount,account.getId()) << endl;
    string msg = stringStream.str();
    logSafe(msg);

    //TODO make sure the order is non relevant on unlock
    bankAccount.leaveWrite();
    account.leaveWrite();
    return taxAmount;
}


