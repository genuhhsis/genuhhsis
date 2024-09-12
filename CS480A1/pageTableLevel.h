/* pageTableLevel.h
 *
 * Genesis Anne Villar (RED ID: 824435476)
 * CS 480 Operating Systems - Prof. Shen
 * Due Date: 9/10/2024
 *
 * File Description:
 *   Defines the PageTableLevel struct and handles implementation details of page table levels
 *   Level is meant to be a node in one of the levels in the page tree
*/
#ifndef PAGETABLELEVEL_H
#define PAGETABLELEVEL_H
#include "log.h"
#include "pageTable.h"

class pageTable; //forward declaration just in case

class pageTableLevel
{
public:
    uint32_t depth; //depth of the node
    static uint32_t pathIndices[32]; //array of indices for the path to the node (i just had a hard max of 32)
    pageTableLevel** nextLevelPtr; //array of pointers to the next level -- array of 2^entrycount[i]
    unsigned int numOfEntries; //number of entries in the next level
    uint32_t numOfAccesses; //number of accesses to this node
    pageTable* pageTablePtr; //pointer to the parent page table

    pageTableLevel(pageTable* parent, unsigned int pageNum, unsigned int depth, unsigned int numOfEntries); //constructor
    ~pageTableLevel(); //destructor
    unsigned int recordPageAccess(uint32_t address);   // record page access (required function)


};

#endif