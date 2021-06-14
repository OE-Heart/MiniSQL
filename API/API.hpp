/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:26
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 17:42:52
 * @Description: 
 * @FilePath: /MiniSQL/API/API.hpp
 */
#pragma once

#include "../Interpreter/Interpreter.hpp"

class MiniSQL{
public:
    MiniSQL();
    ~MiniSQL();
    Interpreter interpreter;
private:
};

namespace API{
    bool createIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName);
    bool dropTable(const std::string &tableName);
    bool dropIndex(const std::string &indexName);
}