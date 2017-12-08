//
// Created by compm on 06/12/17.
//

#ifndef OSWET2_PRINTSAFE_H
#define OSWET2_PRINTSAFE_H

#include <pthread.h>
#include <iostream>

using namespace std;

//call startPrintSafe() before you want to start print thread safe
void startPrintSafe();

//atomic log of msg to log.txt
void logSafe(const string& msg);

//atomic print of msg
void printSafe(const string& msg);

//when you finish all your thread safe prints call finishPrintSafe()
void finishPrintSafe();

#endif //OSWET2_PRINTSAFE_H
