//
// Created by compm on 06/12/17.
//

#include <fstream>
#include "PrintSafe.h"

pthread_mutex_t printMutex;
pthread_mutex_t logMutex;
ofstream logfile;
void startPrintSafe() {
    //TODO verify if init mutex make it unlock by default
    int initLogMutexCheck =  pthread_mutex_init(&logMutex,NULL);
    int initPrintMutexCheck = pthread_mutex_init(&printMutex,NULL);
    if (initPrintMutexCheck) {
        cerr << "pthread_mutex_init failed: Print Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
    if (initLogMutexCheck) {
        cerr << "pthread_mutex_init failed: Log Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }

    //verify open log file
    logfile.open("log.txt");
    if (!logfile.is_open())
    {
        cerr << "Failed to open log.txt file for write." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
}

void printSafe(const string& msg) {
    pthread_mutex_lock(&printMutex);
    cout << msg;
    pthread_mutex_unlock(&printMutex);
}

// TODO - print to log.txt instead cout. createfile at start and close at finish
void logSafe(const string& msg) {
    if (logfile.is_open()) {
        pthread_mutex_lock(&logMutex);
        logfile << msg;
        pthread_mutex_unlock(&logMutex);
    }else{
        cerr << "Failed to write to log.txt file." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
}



void finishPrintSafe() {
    //TODO verify if free mutex correctly
    int destroyPrintMutexCheck = pthread_mutex_destroy(&printMutex);
    int destroyLogMutexCheck = pthread_mutex_destroy(&logMutex);
    //TODO this is how we check valid free mutex
    if (destroyPrintMutexCheck) {
        cerr << "pthread_mutex_destroy failed: Print Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
    if (destroyLogMutexCheck) {
        cerr << "pthread_mutex_destroy failed: Log Mutex." << endl;
        //TODO verify that exit() allowed
        exit(-1);
    }
    //close log.txt file
    logfile.close();
}