
/* pageTableLevel.cpp
 *
 * Genesis Anne Villar (RED ID: 824435476)
 * CS 480 Operating Systems - Prof. Shen
 * Due Date: 9/10/2024
 *
 * */

#include <iostream>
#include "pageTableLevel.h"

// static varible that logs index so that value is retained across all instances of pageTableLevel
uint32_t pageTableLevel::pathIndices[32] = {};

// constructor definition
pageTableLevel::pageTableLevel(pageTable* parent, unsigned int pageNum, unsigned int depth, unsigned int numOfEntries)
{
    //initialize variables when a new pageTableLevel is created
    this->pageTablePtr = parent;
    this->depth = depth;
    this->numOfEntries = numOfEntries;
    this->numOfAccesses = 0;

    nextLevelPtr = new pageTableLevel*[numOfEntries](); //instantianting a double pointer array

    for (unsigned int i = 0; i < numOfEntries; i++) // array of 2^entrycount[i] items of type nextLevel
    {
        nextLevelPtr[i] = nullptr; //each should point to NULL pointer
    }

}

// destructor definition
pageTableLevel::~pageTableLevel()
{
    for (unsigned int i = 0; i < numOfEntries; i++) //iterate through each entry in the nextLevelPtr array
    {
        if (nextLevelPtr[i] != nullptr)
        {
            delete nextLevelPtr[i]; //delete each entry in the nextLevelPtr array
        }
    }
    delete[] nextLevelPtr;
}


// recordPageAccess function definition
/**
             * @brief Record the page access for the address by traversing the corresponding
             * page path in the tree, inserting entries to the page table tree if needed,
             * increment the number of accesses to the nodes / levels traversed, and
             * return number of accesses to the page after reaching the leaf node.
             * Traverse from the current level, creates sublevels as needed.
             * Use a field numOfAccesses for tracking number of accesses to the page / node.
             * Note when inserting a page, do not create and add the level / node if the
             * page entry already exists at the level, just continue to the next level.
             * @param address address being accessed
             * @return unsigned int number of accesses to the page of the address
             * */

unsigned int pageTableLevel::recordPageAccess(uint32_t address)
{
    // calculate the index for this level using the page table's method to ensure correct bit manipulation
    // should use values that have been computed in the pageTable constructor
    uint32_t index = pageTablePtr->extractPageNumberFromAddress(address, pageTablePtr->bitMaskAry[depth], pageTablePtr->shiftAry[depth]);
    pathIndices[depth] = index;

    if (index >= numOfEntries) // check if the index is within the allowed range for this level
    {
        return 0;  // Address index out of bounds
    }

    // if this node does not exist, we must create it
    if (nextLevelPtr[index] == nullptr)
    {
        nextLevelPtr[index] = new pageTableLevel(pageTablePtr, index, depth + 1, pageTablePtr->entryCount[depth + 1]);
    }

    // if this is the leaf level, increment and log access
    if (depth == pageTablePtr->levelCount - 1)
    {
        nextLevelPtr[index]->numOfAccesses++;  // Each leaf node has its own access counter
        // in accordance with log.h ... log output one address at a time: memory address, page indices ,page levels, number of accesses to the page

        log_pgindices_numofaccesses(address, depth + 1, pathIndices, nextLevelPtr[index]->numOfAccesses);
        return nextLevelPtr[index]->numOfAccesses;
    }
    else
    {
        // not a leaf level, recurse deeper
        return nextLevelPtr[index]->recordPageAccess(address);
    }
}





#ifdef STANDALONE
#endif