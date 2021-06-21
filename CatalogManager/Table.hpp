/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 19:24:50
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-21 15:32:08
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Table.hpp
 */
#pragma once

// #define DEBUG

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "Column.hpp"
#include "Block.hpp"

struct Table 
{
    std::string tableName;
    std::vector<Column> columns;
    std::unordered_map<std::string, int> cmaps;
    int blockCnt;
    int recordSize;
    int recordInBlock;

    bool hasColumn(const std::string& columnName) const;
    Column getColumn(const std::string& columnName) const;
    int indexOfCol(const std::string& columnName);
    int size();
    int rib();
    std::string toString() const;
    static Table fromString(const char *&p);
};