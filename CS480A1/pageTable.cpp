/*/
 * Design and implement clean interfaces between modules.
 * Meaningful variable names.
 * NO global variables.
 * NO hard code – Magic numbers, etc.
 * Have proper code structure between .h and.cpp files
 *
 * General outline
 * 1.) construct level bit masks
 * 2.) calculate right shifting to get page number for each level
 * 3.) apply bitwise AND operation using mask, then right shifting
 *
 * bitMaskAry[], shiftArray[], entryCount[] should be computed  dynamically based on pageTable specification
 * memory address = page number * page size + offset into the page.
 * page number = memory address / page size
 * offset into the page = memory address mod page size
 *
 */

#include <iostream>
#include "pageTable.h"
#include "pageTableLevel.h"


// constructor definition
pageTable::pageTable(unsigned int levelCount, unsigned int *bitLength)
{
    if (levelCount > 0) //if level count is greater than 0 (valid entry recognized), initialize all variables
    {
        this->levelCount = levelCount;
        bitMaskAry = new unsigned int[levelCount];
        shiftAry = new unsigned int[levelCount];
        entryCount = new unsigned int[levelCount];
        unsigned int shift = 32 ; //  32-bit system, each memory address has 32 bits

        // in a 32 bit system, sum of all bits should not exceed 32
        unsigned int totalBits = 0;

        /*
         * if 4bits 8bits 8bits for level 3 tree
         *  level 0: 32 - 4 = 28 (store in shiftAry[0])
         *  1 << 4 = 16 (10000) - 1 ---> 1111 (needed bit mask)
         *  each level can have 2^n entries (n = #bits)
         *  soo... 1 << 4 = 16 (store in entryCount[0])
         */

        // since we are working with a 32-bit system, we need to ensure that the total number of bits does not exceed 32
        for (unsigned int i = 0; i < levelCount; i++) //iterate through each level
        {
            totalBits = totalBits + bitLength[i]; //add the number of bits at each level

            if (totalBits > 32) //if total bits exceeds 32, exit program
            {
                exit(EXIT_FAILURE);
            }


            // calculate the shift and bit mask for each level if total bits is less than 32 (valid entry)
            shift = shift - bitLength[i]; // based #bits at this level, decrease shift

            if(shift < 0)
            {
                exit(EXIT_FAILURE); //exit program if shift is less than 0
            }
            bitMaskAry[i] = ((1 << bitLength[i]) - 1) << shift; //create bit mask for level i in correct position
            shiftAry[i] = shift; //set shift for extracting page number for level i
            entryCount[i] = 1 << bitLength[i]; //number of next level entries for level i
        }
        rootNodePtr = new pageTableLevel(this, 0, 0, entryCount[0]); //initialize root node
    }
    else
    {
        exit(EXIT_FAILURE); //exit program if level count is less than 0
    }
}

// destructor definition
pageTable::~pageTable()
{
    delete[] bitMaskAry;
    delete[] shiftAry;
    delete[] entryCount;
}


/**
 * @brief     Given an address, apply the given bit mask and shift right by the given number
             * of bits. Returns the page number or index. This function can be used to extract
             * the page number (index) of any page level or the full-page number by
             * supplying the appropriate parameters.
             * @param address
             * @param mask
             * @param shift
             * @return extracted part of address based on mask and shift
             */

//changed address to uint32_t so that it can match with the log.h types
unsigned int pageTable::extractPageNumberFromAddress(uint32_t address, unsigned int mask, unsigned int shift)
{
    //isolate the bits of the address
    uint32_t maskedAddress = address & mask; //apply the bitmask to the address

    if(shift >= sizeof(address) * 8)
    {
        return 0; //return 0 if shift is greater than the number of bits in the address
    }
    //shift the masked address to the right
    maskedAddress = maskedAddress >> shift; //shift the masked address to the rightmost by the given number of bits

    return maskedAddress; //return the shifted address
}


unsigned int pageTable::recordPageAccess(uint32_t address) const
{
    if (rootNodePtr != nullptr)
    {
        return rootNodePtr->recordPageAccess(address);  // delegate to the root node
    }
    return 0;  // if the root node is null, return 0 (error case)
}