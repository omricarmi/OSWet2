//
// Created by compm on 01/12/17.
//


#include "ATM.h"

//TODO add new log error when account doesn't exists !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
        stringStream << ACCOUNT_NOT_EXISTS(atmId,accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //check correct password
    if(!(*itFrom).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //in case target does not exists
    if( itTo==accounts.end() ){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << ACCOUNT_NOT_EXISTS(atmId,targetAccountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //try to make the transaction
    TransferData data = (*itFrom).second.transfer(transferAmount,(*itTo).second);
    if(data.status == 1){
        //log success transfer
        std::ostringstream stringStream;
        //Example (is one line): <ATM ID>: Transfer <amount> from account <account> to account <target_account>
        // new account balance is <account_bal> new target account balance is <target_bal>
        stringStream << atmId << ": Transfer " << transferAmount << " from account " << accountId << " to account "
                     << targetAccountId << " new account balance is " << data.fromNewBalance
                     << " new target account balance is "<< data.toNewBalance
                     << endl;
        string msg = stringStream.str();
        logSafe(msg);
        return;
    }else{
        //on transfer failed log it
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – account id <id> balance is lower than <amount>
        stringStream << "Error " << atmId << ": Your transaction failed – account id " <<
                     accountId << " balance is lower than " << transferAmount << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
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
        stringStream << ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    //check if password correct
    if(!(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }
    //log the desired account balance
    int balance = (*it).second.getBalance();
    std::ostringstream stringStream;
    //Example: <ATM ID>: Account <id> balance is <bal>
    stringStream << atmId << ": Account " << accountId << " balance is " << balance << endl;
    string msg = stringStream.str();
    logSafe(msg);
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
        stringStream << ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    // for incorrect password ,log error
    if(!(*it).second.verifyPassword(password)){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }

    //try to withdraw
    int withdrawResult = (*it).second.draw(amount);
    // in case the withdraw amount is bigger than the current balance
    if ( withdrawResult == -1 ){
        std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – account id " << accountId << " balance is lower than " << amount << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }

    std::ostringstream stringStream;
    // log for success withdrawal
    //Example:<ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew
    stringStream << atmId << ": Account " << accountId << " new balance is " << withdrawResult << " after " << amount << " $ was withdrew" << endl;
    string msg = stringStream.str();
    logSafe(msg);


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
        stringStream << ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    // for incorrect password ,log error
    if(!(*it).second.verifyPassword(password)){
    std::ostringstream stringStream;
        //Example: Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        stringStream << "Error " << atmId << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
        return;
    }

    // deposit
    int newBalance = (*it).second.deposit(amount);
    std::ostringstream stringStream;
    //Example: <ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited 
    stringStream << atmId << ": Account " << accountId << " new balance is " << newBalance << " after " << amount << " $ was deposited" << endl;
    string msg = stringStream.str();
    logSafe(msg);


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
        stringStream << ACCOUNT_NOT_EXISTS(atmId, accountId) << endl;
        string errMsg = stringStream.str();
        logSafe(errMsg);
    }
    //for incorrect password
    if(!(*it).second.verifyPassword(password)){
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
