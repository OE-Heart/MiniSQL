/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:43:05
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 10:55:05
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Column.hpp
 */
#pragma once

#include <string>
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
    
}