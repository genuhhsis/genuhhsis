/* pageTable.h
 *
 * Genesis Anne Villar (RED ID: 824435476)
 * CS 480 Operating Systems - Prof. Shen
 * Due Date: 9/10/2024
 *
 * File Description:
 *   Defines the PageTable struct and control logic for managing overall page table
 *   All variables on pageTable are unsigned integers for easier bit manipulation
*/
#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "log.h"
#include "pageTableLevel.h"

class pageTableLevel; //forward declaration just in case

class pageTable
        {
        public:
            pageTable(unsigned int levelCount, unsigned int *bitLength); //constructor
            ~pageTable(); //destructor
            pageTableLevel* rootNodePtr; //pointer to the root node of the page table
            unsigned int recordPageAccess(unsigned int address) const; //record page access (required function)
            unsigned int extractPageNumberFromAddress(uint32_t address, unsigned int mask, unsigned int shift); //extract page number from address (required function)


            // outline obtained from pagetable.pdf
            unsigned int levelCount; // number of levels in the page table (max 10)

            // each variable is a DYNAMIC ARRAY of unsigned integers
            unsigned int *bitMaskAry; //bit mask for level i
            unsigned int *shiftAry; //#of bits to shift for getting level i page index
            unsigned int *entryCount; //# of next level entries for level i
        };

#endif