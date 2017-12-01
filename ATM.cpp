//
// Created by compm on 01/12/17.
//

#include "ATM.h"

void* atmThreadWrapper(void *pAtmThreadData) {
    //extract data from caller
    string inputFileName = ((AtmThreadData*)pAtmThreadData)->inputFileName;
    int atmId = ((AtmThreadData*)pAtmThreadData)->atmId;

    //TODO read text file and fulfill orders


    return nullptr;
}
