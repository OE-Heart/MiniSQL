/**
 * @file Block.cpp
 * @author Yinwhe
 * @brief Implementation of the Block
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "Block.hpp"
// #define DEBUG
Block::Block(){
    /* Initialization */
    memset(data, 0, BLOCK_SIZE);
    isPinned = false;
    isDirty = false;
    uptodate = false;
    filename = "";
    offset = 0;
    refcnt = 0;

    #ifdef DEBUG
    printf("New Block!\n");
    #endif
}