/* main.cpp
 *
 * Genesis Anne Villar (RED ID: 824435476)
 * CS 480 Operating Systems - Prof. Shen
 * Due Date: 9/10/2024
 *
 * File Description:
 *   Handles initialization of page table and page table levels structures
 *    -- this includes input (reading memory address) and output (printing page table)
*/

#include <iostream>
#include <cstring>
#include "pageTable.h"
#include "log.h"
#include "tracereader.h"



int main(int argc, char** argv) //argc is the number of arguments, argv is the array of arguments
{

    //check if there are enough arguments in command line
    if (argc < 3)
    {
        std::cout << "expecting at least 3 arguments\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    // argv[0] is ./pagetrace
    // argv[1] is the trace file (trace.tr)
    // argv[2] is the number of levels in the page table, based on bit configuration

    char* bitConfig = argv[2];

    // iterate through each character in the input string and add to levelCount based on how many digits are in the command input
    int levelCount = 0; //initialize level count
    unsigned int bits[10]; //initialize bits array (i just added 10 as a placeholder)

    char* s = strtok(bitConfig, " "); //continue to tokenize the string until there are no more spaces
    while (s != nullptr)
    {
        // convert string to int
        bits[levelCount++] = std::stoi(s); //increment level count while there are still characters in the string
        s = strtok(nullptr, " ");
    }

    //initialize pageTable object
    pageTable* pageTablePtr = new pageTable(levelCount, bits);

    //call log_bitmask function to log bit mask values
    log_bitmasks(levelCount, pageTablePtr->bitMaskAry);

    //open 'trace.tr' file on argv[1] -- in read mode
   FILE *file = fopen(argv[1], "r");

   // check if file was opened successfully, if not, print error message and exit
    if (file == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    //define structure to hold address trace data (followed outline on a1.pdf)
    p2AddrTr trace;

    // continuously read address traces from the file until there are no more to read
    while (NextAddress(file, &trace))
    {
         pageTablePtr->recordPageAccess(trace.addr);
    }


    // close trace file + clean pageTable object
    fclose(file);
    delete pageTablePtr;
    return EXIT_SUCCESS;
}
