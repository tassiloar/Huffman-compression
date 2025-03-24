

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include <stdexcept>
#include "HuffmanCoder.h"
#include "BinaryIO.h"
#include <cstring>

/*
main
Input: inputs num and array of pointer to string input files
Purpose: Runs program by creating a HuffmanCoder object and calling 
teh appropriate function
*/
int main(int argc, char const *argv[])
{   
    if (argc != 4 or not (strcmp(argv[1], "zap") == 0 or 
    strcmp(argv[1], "unzap") == 0)) {
        cerr << "Usage: ./zap [zap | unzap] inputFile outputFile" << endl;
        return EXIT_FAILURE;
    }

    HuffmanCoder coder;

    if (strcmp(argv[1], "zap") == 0) {

        coder.encoder(argv[2],argv[3]);

    } else if (strcmp(argv[1], "unzap") == 0) {

        coder.decoder(argv[2],argv[3]);
    }

    return 0;
}

