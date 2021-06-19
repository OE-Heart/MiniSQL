/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 19:24:50
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 10:31:48
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Table.hpp
 */
#include <string>
#include <vector>
#include <algorithm>
#include "Column.hpp"

struct Table 
{
    std::string tableName;
    std::vector<Column> columns;
    int recordCnt;
    int blockCnt;

    bool hasColumn(const std::string& columnName) const;
    Column getColumn(const std::string& columnName) const;
    int size() const;
    std::string toString() const;
};

bool Table::hasColumn(const std::string& columnName) const
{
    
}

Column Table::getColumn(const std::string& columnName) const
{
    
}

int Table::size() const
{
    
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