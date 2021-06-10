/*
 * @Author: Ou Yixin
 * @Date: 2021-06-10 10:04:21
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 14:38:31
 * @Description: 
 * @FilePath: /MiniSQL/Interpreter/MiniSQLcmd.hpp
 */
#pragma once

#include <string>
#include <vector>
#include <variant>

#include "../CatalogManager/Column.hpp"

enum class Operator {
    eq,   //=
    neq,  //!=
    l,    //<
    leq,  //<=
    g,    //>
    geq   //>=
};

class Condition
{
private:
    std::string columnName;
    Operator op;
    
public:
    Condition(/* args */);
    ~Condition();
};

Condition::Condition(/* args */)
{
}

Condition::~Condition()
{
}

class createTableCmd {
    std::string tableName;
    std::vector<Column> columns;
};

class dropTableCmd {
    std::string tableName;
};

class createIndexCmd {
    std::string indexName;
    std::string tableName;
    std::string columnName;
};

class dropIndexCmd {
    std::string indexName;
};

class insertCmd {
    std::string tableName;
};

class deleteCmd {
    std::string tableName;
};

class selectCmd {
    std::string tableName;
    bool selectAll = false;
    std::vector<std::string> columnNames;

};

class exitCmd {
    
};