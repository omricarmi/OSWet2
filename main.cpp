//
// Created by compm on 27/11/17.
//

#include <pthread.h>
#include <stdlib.h>
#include <map>
#include <unistd.h>
#include <iostream>
#include "common.h"
#include "Account.h"
#include "Bank.h"
#include "ATM.h"

using namespace std;

/***** Function Declaration *****/
void* statusThreadWrapper(void* data);

//TODO make makefile with -lpthread and c++11 flags, check what else needed

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

    //must init to able to use thread safe print
    startPrintSafe();

    //must call before using bank
    startBank();

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
        pthread_join(atmThreads[j],NULL);
    }

    //tell everyone with global flag that ATMs Threads finished
    isATMsFinished = true;
    // wait for status thread after Bank finished
    pthread_join(statusThread,NULL);
    isStatusFinished = true;
    // wait for bank thread after Status finished
    pthread_join(bankThread,NULL);

    /***** Not Thread Safe - END *****/

////DEBUG - START
//    //demo create accounts
//    for(int i=1;i<20;i+=4){
//        Account *acc = new Account(100-i,i,i*34);
//        accounts.insert(pair<AccountId,Account&>(100-i,*acc));
//    }
//    for(int i=2;i<20;i+=4){
//        Account *acc = new Account(100-i,i,i*284);
//        accounts.insert(pair<AccountId,Account&>(100-i,*acc));
//    }
//    for(int i=0;i<20;i+=4){
//        Account *acc = new Account(100-i,i,i*17);
//        accounts.insert(pair<AccountId,Account&>(100-i,*acc));
//    }
//    printSafe(getAccountsStatus(getBankAccount()));
////DEBUG - END

    //finish bank
    finishBank();

    //finish safe thread print
    finishPrintSafe();

    return 0;
}

/***** Helper methods *****/
void* statusThreadWrapper(void* data){
    const int halfSecond = 500000;//5e5 micro sec
    while(!isATMsFinished) {
        usleep(halfSecond);
        //print the status of accounts and the bank
        string status = getAccountsStatus(getBankAccount());
    }
    return NULL;
}