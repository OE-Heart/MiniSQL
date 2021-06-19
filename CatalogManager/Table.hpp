/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 19:24:50
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 11:13:00
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Table.hpp
 */
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
};

bool Table::hasColumn(const std::string& columnName) const
{
    for (const auto &col : columns) {
        if (col.columnName == columnName)
            return true;
    }
    return false;
}

Column Table::getColumn(const std::string& columnName) const
{
    for (const auto &col : columns) {
        if (col.columnName == columnName) {
            return col;
        }
    }
    return Column{};
}

int Table::indexOfCol(const std::string& columnName){
    if(cmaps.empty()){// Create map of column names
        for(int i=0;i<columns.size();i++){
            if(columns[i].columnName == columnName) cmaps[columnName] = i;
        }
    }
    return cmaps.at(columnName);
}

int Table::size()
{
    if (recordSize > 0) {
        return recordSize;
    } else {
        recordSize = 0;
        for (const auto &col : columns) {
            recordSize += col.size();
        }
        return recordSize;
    }
}

std::string Table::toString() const
{
    std::string s;
    int nameSize = tableName.size();
    std::copy_n(reinterpret_cast<char *>(&nameSize), sizeof(int), std::back_inserter(s));
    s += tableName;

    int columnsSize = columns.size();
    std::copy_n(reinterpret_cast<char *>(&columnsSize), sizeof(int), std::back_inserter(s));
    std::copy_n(reinterpret_cast<const char*>(&recordCnt), sizeof(int), std::back_inserter(s));

    for (const auto &column : columns)
    {
        s += column.toString();
    }

    return s;
}