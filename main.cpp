//
// Created by compm on 27/11/17.
//

#include <stdlib.h>
#include <unistd.h>
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

//TODO make makefile with -lpthread and c++11 flags, check what else needed
//TODO notice prints arent atomic and they need mutex !!!
//TODO watch out to use reference to change global variables like accounts !!!

/***** Main Entry *****/
int main(int argc, char* argv[]){

    //validate program minimum param
    if(argc<2){
        cout << ERR_FAILED_RUN_ATTEMPT << endl;
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

    /***** Not Thread Safe - END *****/
    // TODO verify that main ending destruct accounts dynamically allocated in openAccount in ATM.cpp

////DEBUG - START
//    //demo create accounts
//    for(int i=1;i<20;i+=4){
//        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
//    }
//    for(int i=2;i<20;i+=4){
//        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
//    }
//    for(int i=0;i<20;i+=4){
//        accounts.insert(std::make_pair<int,Account>(100-i,Account(100-i,i)));
//    }
////DEBUG - END

    //finish safe thread print
    finishPrintSafe();

    return 0;
}

/***** Helper methods *****/
void* statusThreadWrapper(void* data){
    bool isContinue = true;
    const int halfSecond = 500000;//5e5 micro sec
    while(isContinue) {
        usleep(halfSecond);
        //clear screen
        printSafe("\033[2J");
        //move cursor to left up corner
        printSafe("\033[1;1H");
        //print the status of accounts and the bank
        string status = getAccountsStatus(getBankAccount());
        printSafe(status);
    }
}