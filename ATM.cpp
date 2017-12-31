//
// Created by compm on 01/12/17.
//


#include "ATM.h"


void openAccount(vector<string> words, int atmId);

void makeVip(vector<string> words, int atmId);

void deposit(vector<string> words, int atmId);

void withdraw(vector<string> words, int atmId);

void getBalance(vector<string> words, int atmId);

void transfer(vector<string> words, int atmId);

void* atmThreadWrapper(void *pAtmThreadData) {
    //extract data from caller
    string inputFileName = ((AtmThreadData*)pAtmThreadData)->inputFileName;
    int atmId = ((AtmThreadData*)pAtmThreadData)->atmId;

    //read text file and fulfill orders
    string line;
    ifstream myfile (inputFileName);
    if (myfile.is_open())
    {
        vector<string> words;
        char delim = ' ';
        while ( getline (myfile,line) )
        {
            words = split(line, delim);
            if(words[0] == "O"){
                openAccount(words, atmId);
            }else if(words[0] == "L"){
                makeVip(words, atmId);
            }else if(words[0] == "D"){
                deposit(words, atmId);
            }else if(words[0] == "W"){
                withdraw(words, atmId);
            }else if(words[0] == "B"){
                getBalance(words, atmId);
            }else if(words[0] == "T"){
                transfer(words, atmId);
            }
            //make 0.1 sec delay
            usleep(100000);
        }
        myfile.close();
    }
    else{
        cerr << "failed to open file: " << inputFileName << endl;
        exit(-1);
    }
    return NULL;
}

void transfer(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int targetAccountId = stoi(words[3]);
    int transferAmount = stoi(words[4]);
    // in case source account doesn't exist or incorrect password ,log error
    auto itFrom = accounts.find(accountId);
    auto itTo = accounts.find(targetAccountId);
    //check from account exists
    if(itFrom==accounts.end()){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> does not exist
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //check correct password
    if(!(*itFrom).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_WRONG_PASSWORD(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //in case target does not exists
    if( itTo==accounts.end() ){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId,targetAccountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //try to make the transaction
    (*itFrom).second.transfer(transferAmount, (*itTo).second, atmId);
}

void getBalance(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    // in case the account don't exist or incorrect password ,log error
    auto it = accounts.find(accountId);
    //check account exists
    if(it==accounts.end()) {
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> does not exist
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    //check if password correct
    if(!(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_WRONG_PASSWORD(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //log the desired account balance
    (*it).second.getBalance(atmId);
}

void withdraw(vector<string> words, int atmId) {

    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int amount = stoi(words[3]);

    auto it = accounts.find(accountId);
    //check account exists
    if(it==accounts.end()) {
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> does not exist
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    // for incorrect password ,log error
    if(!(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_WRONG_PASSWORD(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }

    //try to withdraw
    (*it).second.draw(amount, atmId);
}

void deposit(vector<string> words, int atmId) {

    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int amount = stoi(words[3]);

    auto it = accounts.find(accountId);
    //check account exists
    if(it==accounts.end()) {
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> does not exist
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    // for incorrect password ,log error
    if(!(*it).second.verifyPassword(password)){
    std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_WRONG_PASSWORD(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    // deposit
    (*it).second.deposit(amount, atmId);
}

void makeVip(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    // in case the account doesn't exist or incorrect password ,log error
    auto it = accounts.find(accountId);
    //check account exists
    if(it==accounts.end()) {
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> does not exist
        stringStream << LOG_ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    //for incorrect password
    if(!(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << LOG_WRONG_PASSWORD(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //set account to be VIP
    (*it).second.setVIP(true);
}

void openAccount(vector<string> words, int atmId) {

    //prevent adding 2 accounts simultaneously
    lockAddAccount();

    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int initialAmount = stoi(words[3]);
    // in case there is already an account with the same id ,log error
    if(accounts.find(accountId) != accounts.end() ){
        unlockAddAccount();
        std::ostringstream stringStream;
        stringStream << LOG_ACCOUNT_ALREADY_EXISTS(atmId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }

    //make 1 sec delay
    sleep(1);
    //init the account
    Account *newAccount = new Account(accountId, password,initialAmount);
    // print creation success
    std::ostringstream stringStream;
    char strPassword[5];
    sprintf(strPassword,"%04d",password);
    stringStream <<  LOG_ACCOUNT_CREATED(atmId,accountId,strPassword,initialAmount) <<endl;
    string msg = stringStream.str();
    logSafe(msg);
    //add the account to the global map
    accounts.insert(pair<AccountId,Account&>(accountId,*newAccount));

    //end of adding account so can release the lock
    unlockAddAccount();
}
