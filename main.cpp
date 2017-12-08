//
// Created by compm on 27/11/17.
//

#include <stdlib.h>
#include <iostream>
#include "common.h"
#include "ATM.h"
#include "Account.h"
#include "Bank.h"
#include "PrintSafe.h"
#include <pthread.h>
#include <map>

using namespace std;


/***** Function Declaration *****/
void* statusThreadWrapper(void* data);

//TODO notice prints arent atomic and they need mutex !!!
//TODO watch out to use reference to change global variables like accounts !!!

/***** Main Entry *****/
int main(int argc, char* argv[]){

    //validate program minimum param
    if(argc<2){
        cout << FAILED_RUN_ATTEMPT;
        //TODO verify correct status exit number
        return 1;
    }

    //parse params
    int N = atoi(argv[1]); //amount of ATMs
    char** inputFiles = &argv[2]; //input file names for ATMs

    //must init to able to use thread safe print //TODO (remember to release in the end)
    startPrintSafe();

    /***** Not Thread Safe - START *****/

    //init threads for ATMs
    pthread_t atmThreads[N];
    AtmThreadData atmThreadsData[N];
    for (int i = 0; i < N; ++i) {
        atmThreadsData[i].atmId = i+1;
        atmThreadsData[i].inputFileName = inputFiles[i];
        pthread_create(&atmThreads[i],NULL,atmThreadWrapper,(void*)(&atmThreadsData[i]));
    }

    //init thread for Bank
    pthread_t bankThread;
    BankThreadData bankThreadData;
    bankThreadData.pBankAccount = &bankAccount;
    pthread_create(&bankThread,NULL,bankThreadWrapper,(void*)(&bankThreadData));

    //init thread that print the current state of the bank
    pthread_t statusThread;
    pthread_create(&statusThread,NULL,statusThreadWrapper,NULL);

    //wait for ATMs threads to finish
    for (int j = 0; j < N; ++j) {
        //TODO check if need status return
        pthread_join(atmThreads[j],NULL);
    }

    // TODO kill bank thread after ATMs finished
    // TODO kill status thread after Bank finished
    // TODO destruct accounts

    /***** Not Thread Safe - END *****/
//DEBUG - START
    //demo create accounts
    for(int i=1;i<20;i+=4){
        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
    }
    for(int i=2;i<20;i+=4){
        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
    }
    for(int i=0;i<20;i+=4){
        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
    }
    //demo print status
    Log() << getAccountsStatus(accounts,bankAccount);
//DEBUG - END

    //finish safe thread print
    finishPrintSafe();

    return 0;
}

/***** Helper methods *****/
void* statusThreadWrapper(void* data){
    //TODO print the status of accounts and the bank
    //clear screen
    cout << "\033[2J";
    //move cursor to left up corner
    cout << "\033[1;1H";

}