/**
 * @file BufferManager.hpp
 * @author Yinwhe
 * @brief Definition of BufferManager
 * @version 0.1
 * @date 2021-06-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <string>
#include "Block.hpp"

typedef int BID;
const int BLOCK_NUM = 0x1000;

class BufferManager{
private:
    Block *blocks;
    BID getblk(std::string &, int);
    void Bpanic(const char *)const;
public:
    BufferManager();
    ~BufferManager();
    void pinBlock(BID bid);
    void dirtBlock(BID bid);
    void unpinBlock(BID bid);
    void undirtBlock(BID bid);
    BID  bread(std::string &, int);
    bool bwrite(BID bid);
    void brelease(BID bid);
    char*baddr(BID bid);
    void bflush(std::string &str);
};