/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:43:05
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 11:22:01
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

class Column {
    std::string columnName;
    Field field;
    bool isPrimaryKey = false;
    bool isUnique = false;
};