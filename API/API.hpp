/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:26
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-18 15:47:50
 * @Description: 
 * @FilePath: /MiniSQL/API/API.hpp
 */
#pragma once

#include "../Interpreter/Interpreter.hpp"
#include "../CatalogManager/CatalogManager.hpp"
#include "../Interpreter/Condition.hpp"

class MiniSQL{
public:
    MiniSQL();
    ~MiniSQL();
    Interpreter interpreter;
private:
};

namespace API{
    bool createTable(const std::string &tableName, const std::vector<Column> &columns, const std::string &primaryKey);
    bool createIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName);
    bool dropTable(const std::string &tableName);
    bool dropIndex(const std::string &indexName);
    bool insertOn(const std::string &tableName, std::vector<Value> &valueList);
    bool deleteFrom(const std::string &tableName, std::vector<Condition> &conditionList);
    bool select(const std::string &tableName, std::vector<Condition> &conditionList);

    static CatalogManager *cm;
    CatalogManager *getCatalogManager();
    
}