/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:43:05
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 13:49:57
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