//
// Created by compm on 06/12/17.
//

#include "PrintSafe.h"

pthread_mutex_t printMutex;

void startPrintSafe() {
    //TODO verify if init mutex make it unlock by default
    int initPrintMutexCheck = pthread_mutex_init(&printMutex,NULL);
    if (initPrintMutexCheck) {
        cerr << "pthread_mutex_init failed: Print Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
}

void printSafe(const string& msg) {
    pthread_mutex_lock(&printMutex);
    cout << msg;
    pthread_mutex_unlock(&printMutex);
}

void finishPrintSafe() {
    //TODO verify if free mutex correctly
    int destroyPrintMutexCheck = pthread_mutex_destroy(&printMutex);
    //TODO this is how we check valid free mutex
    if (destroyPrintMutexCheck) {
        cerr << "pthread_mutex_destroy failed: Print Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
}