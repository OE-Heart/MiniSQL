/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:43:05
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 20:32:40
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Column.hpp
 */
#pragma once

#include <string>

enum class Field {
    INT,
    FLOAT,
    CHAR,
};

struct Column {
    std::string columnName;
    Field field;
    bool isPrimaryKey = false;
    bool isUnique = false;
    std::string index = "";
    int size() const;
    std::string toString() const;
};

int Column::size() const
{
    
}

std::string Column::toString() const
{
    
}