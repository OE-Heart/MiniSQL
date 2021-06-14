/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 19:24:50
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 19:52:38
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Table.hpp
 */
#include <string>
#include <vector>
#include "Column.hpp"

struct Table{
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
    
}