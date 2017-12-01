//
// Created by compm on 27/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "consts.h"
#include "ATM.h"
#include "Account.h"
#include "Bank.h"
#include <pthread.h>
#include <map>

using namespace std;

/***** GLOBALS *****/
//list of bank accounts
std::map<int,Account> accounts;
Bank bank;

int main(int argc, char* argv[]){

    //validate programm min param
    if(argc<2){
        cout << "too few params\n";
        return 1;
    }

    //parse params
    int N = atoi(argv[1]); //amount of ATMs
    char** inputFiles = &argv[2]; //input file names for ATMs

    //init threads for ATMs
    pthread_t atmThreads[N];
    AtmThreadData atmThreadsData[N];
    for (int i = 0; i < N; ++i) {
        atmThreadsData[i].atmId = i+1;
        atmThreadsData[i].inputFileName = inputFiles[i];
        pthread_create(&atmThreads[i],NULL,atmThreadWrapper,(void*)(&atmThreadsData[i]));
    }

    //TODO init thread for Bank
    pthread_t bankThread;
    BankThreadData bankThreadData;
    bankThreadData.pBank = &bank;
    pthread_create(&bankThread,NULL,bankThreadWrapper,(void*)(&bankThreadData));

    // a thread that print the current state of the bank
    pthread_t statusThread;
    pthread_create(&statusThread,NULL,statusThreadWrapper,NULL);


    //wait for ATMs threads to finish
    for (int j = 0; j < N; ++j) {
        //TODO check if need status return
        pthread_join(atmThreads[j],NULL);
    }

    // TODO kill bank thread after ATMs finished
    // TODO kill status thread after Bank finished

//DEBUG
    for(int i=0;i<N;++i){
//        string msg = inputFiles[i] + "\n";
//        logd(msg);
    }

    return 0;
}

void* statusThreadWrapper(void* data){
    BankThreadData *pBankThreadData = (BankThreadData*)data;
    Bank *pBank = pBankThreadData->pBank;
    //TODO print the status of accounts and the bank

}