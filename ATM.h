//
// Created by compm on 01/12/17.
//

#ifndef OSWET2_ATM_H
#define OSWET2_ATM_H
#include <fstream>
#include <iostream>
#include <vector>
#include "common.h"
#include <string>
#include "Bank.h"
#include "PrintSafe.h"
using namespace std;

typedef struct _AtmThreadData {
    int atmId;
    string inputFileName;
} AtmThreadData;

void* atmThreadWrapper(void* pAtmThreadData);

class ATM {

private:

public:


};


#endif //OSWET2_ATM_H
