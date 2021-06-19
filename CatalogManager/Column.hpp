/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:43:05
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 12:21:00
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Column.hpp
 */
#pragma once

#include <string>
#include <algorithm>
#include <variant>

using Value = std::variant<int, double, std::string>;

enum class Field 
{
    INT,
    FLOAT,
    CHAR,
};

struct Column 
{
    std::string columnName;
    Field field;
    bool isPrimaryKey = false;
    bool isUnique = false;
    int charSize;
    std::string index = "";
    int size() const;
    std::string toString() const;
    static Column fromString(const char *&p);
};

int Column::size() const
{
    if (field == Field::INT)
        return sizeof(int);
    else if (field == Field::FLOAT)
        return sizeof(double);
    else
        return (charSize+1)*sizeof(char);
}

std::string Column::toString() const
{
    std::string s;
    int nameSize = columnName.size();
    std::copy_n(reinterpret_cast<char *>(&nameSize), sizeof(int), std::back_inserter(s));
    s += columnName;

    int tag = int(field);
    if (isUnique) tag |= 4;
    if (isPrimaryKey) tag |= 8;
    std::copy_n(reinterpret_cast<char *>(&tag), sizeof(int), std::back_inserter(s));
    std::copy_n(reinterpret_cast<const char *>(&charSize), sizeof(int), std::back_inserter(s));
    int indexSize = index.size();
    std::copy_n(reinterpret_cast<const char*>(&indexSize), sizeof(int), std::back_inserter(s));
    s += index;
    return s;
}

Column Column::fromString(const char *&p)
{
    Column column;
    int nameSize = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    std::copy_n(p, nameSize, std::back_inserter(column.columnName));
    p + nameSize;
    int tag = *reinterpret_cast<const int *>(p);
    column.field = Field(tag&3);
    column.isUnique = (tag&4) != 0;
    column.isPrimaryKey = (tag&8) != 0;
    p += sizeof(int);

    column.charSize = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    int indexSize = *reinterpret_cast<const int *>(p);
    p += sizeof(int);
    std::copy_n(p, indexSize, std::back_inserter(column.index));
    p += indexSize;

    return column;
}