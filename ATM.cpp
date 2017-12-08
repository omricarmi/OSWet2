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
    // TODO - what exactly do we get as a file name
    int atmId = ((AtmThreadData*)pAtmThreadData)->atmId;

    //TODO read text file and fulfill orders
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

        }
        myfile.close();
    }
    else{
        cerr << "failed to open file: " << inputFileName << endl;
        exit(-1);
    }




    return nullptr;
}

void transfer(vector<string> words, int atmId) {

}

void getBalance(vector<string> words, int atmId) {

}

void withdraw(vector<string> words, int atmId) {

}

void deposit(vector<string> words, int atmId) {

}

void makeVip(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    // in case the account don't exist or incorrect password ,log error
    auto it = accounts.find(accountId);
    if( it==accounts.end() || !(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //set account to be VIP
    (*it).second.setVIP(true);
}

void openAccount(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int initialAmount = stoi(words[3]);
    // in case there is already an account with the same id ,log error
    if(accounts.find(accountId) != accounts.end() ){

        std::ostringstream stringStream;
        stringStream << "Error " << atmId << ": Your transaction failed – account with the same id exists" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //init the account
    Account *newAccount = new Account(accountId, password,initialAmount);
    //add the account to the global map
    accounts.insert(pair<AccountId,Account&>(accountId,*newAccount));
    // print creation success
    std::ostringstream stringStream;
    stringStream <<  atmId <<": New account id is " << accountId <<" with password " << password <<" and initial balance "<< initialAmount <<endl;
    string msg = stringStream.str();
    logSafe(msg);

}
