//
// Created by compm on 01/12/17.
//


#include "ATM.h"

void openAccount(vector<string> words, int i);

void makeVip(vector<string> words, int i);

void deposit(vector<string> words, int i);

void withdraw(vector<string> words, int i);

void getBalance(vector<string> words, int i);

void transfer(vector<string> words, int i);

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

void transfer(vector<string> words, int i) {

}

void getBalance(vector<string> words, int i) {

}

void withdraw(vector<string> words, int i) {

}

void deposit(vector<string> words, int i) {

}

void makeVip(vector<string> words, int i) {

}

void openAccount(vector<string> words, int atmId) {
    int accountId = stoi(words[1]);
    int password = stoi(words[2]);
    int initialAmount = stoi(words[3]);
    // in case there is already an account with the same id
    if(accounts.find(accountId) != accounts.end() ){

        std::ostringstream stringStream;
        "Error <ATM ID>: Your transaction failed – account with the same id exists"
        stringStream << "Error " << atmId << ": Your transaction failed – account with the same id exists" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    Account *newAccount = new Account(accountId, password,initialAmount);
}
