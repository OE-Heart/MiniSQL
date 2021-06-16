/**
 * @file Block.hpp
 * @author Yinwhe
 * @brief Implementation of the Block
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <string>

/* Page size */
const int BLOCK_SIZE = 0x1000;

struct Block {
    char data[BLOCK_SIZE];  // Store data
    bool isPinned;          // Is pinned?
    bool isDirty;           // Is dirty?
    bool uptodate;          // uptodated?
    std::string filename;
    int offset;             // Offset is similar to file pointer in files
    int refcnt;             // Number of reference
    
    Block();
};