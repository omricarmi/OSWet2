//
// Created by compm on 27/11/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//TODO declare list of bank accounts

int main(int argc, char* argv[]){

    //validate programm min param
    if(argc<2){
        cout << "too few params\n";
        return 1;
    }

    int N = atoi(argv[1]);
    char** inputFiles = &argv[2];

//DEBUG
    for(int i=0;i<N;++i){
        printf("%s\n",inputFiles[i]);
    }

    //TODO on 28/11/17 by Omri Carmi : init ATMs pthreads


    return 0;
}