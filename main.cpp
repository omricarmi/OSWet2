//
// Created by compm on 27/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "consts.h"
#include "ATM.h"
#include "Account.h"
#include <pthread.h>
#include <map>

using namespace std;

/***** GLOBALS *****/
//list of bank accounts
std::map<int,Account> accounts;

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
    

    //wait for ATMs threads to finish
    for (int j = 0; j < N; ++j) {
        //TODO on 01/12/17 by Omri Carmi : check if need status return
        pthread_join(atmThreads[j],NULL);
    }

//DEBUG
    for(int i=0;i<N;++i){
//        string msg = inputFiles[i] + "\n";
//        logd(msg);
    }

    //TODO on 28/11/17 by Omri Carmi : init ATMs pthreads


    return 0;
}