/*
 * @Author: Yinwhe
 * @Date: 2021-06-19 13:50:11
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-20 12:18:20
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#include "Table.hpp"

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
            cmaps[columns[i].columnName] = i;
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
    #ifdef DEBUG
    std::cout << "nameSize: " << nameSize << "\n";
    #endif // DEBUG
    std::copy_n(reinterpret_cast<char *>(&nameSize), sizeof(int), std::back_inserter(s));
    s += tableName;
    int columnsSize = columns.size();
    std::copy_n(reinterpret_cast<char *>(&columnsSize), sizeof(int), std::back_inserter(s));
    std::copy_n(reinterpret_cast<const char*>(&recordCnt), sizeof(int), std::back_inserter(s));
    std::copy_n(reinterpret_cast<const char*>(&blockCnt), sizeof(int), std::back_inserter(s));

    for (const auto &column : columns)
    {
        s += column.toString();
    }

    #ifdef DEBUG
    std::cout << "tableString(in): " << s << "\n";
    #endif

    return s;
}

Table Table::fromString(const char *&p)
{
    Table table;
    int nameSize = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    std::copy_n(p, nameSize, std::back_inserter(table.tableName));
    p += nameSize;
    int columnsSize = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    table.recordCnt = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    table.blockCnt = *reinterpret_cast<const int *>(p);
    p += sizeof(int);

    for (int i = 0; i < columnsSize; i++)
    {
        Column column = Column::fromString(p);
        table.columns.emplace_back(column);
    }

    table.recordSize = 0;

    return table;
}