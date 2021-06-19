/*
 * @Author: Yinwhe
 * @Date: 2021-06-16 09:50:16
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 14:44:27
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#pragma once

#include <cstring>
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