/*
 * @Author: Ou Yixin
 * @Date: 2021-06-18 14:59:10
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-18 15:12:19
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Condition.hpp
 */

#pragma once

#include <string>
#include "../CatalogManager/Column.hpp"

enum class OP
{
    EQ,
    NEQ,
    L,
    LEQ,
    G,
    GEQ
};

struct Condition
{
    std::string columnName;
    OP op;
    Value value;
};