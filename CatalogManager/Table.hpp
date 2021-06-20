/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 19:24:50
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-20 12:17:03
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

struct Table 
{
    std::string tableName;
    std::vector<Column> columns;
    std::unordered_map<std::string, int> cmaps;
    int recordCnt;
    int blockCnt;
    int recordSize;

    bool hasColumn(const std::string& columnName) const;
    Column getColumn(const std::string& columnName) const;
    int indexOfCol(const std::string& columnName);
    int size();
    std::string toString() const;
    static Table fromString(const char *&p);
};