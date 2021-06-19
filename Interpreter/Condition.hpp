/*
 * @Author: Ou Yixin
 * @Date: 2021-06-18 14:59:10
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 13:44:30
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/Condition.hpp
 */

#pragma once

#include <string>
#include "Column.hpp"

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
    bool isTrue(const Value&) const;
};