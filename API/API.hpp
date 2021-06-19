/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:26
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 13:16:40
 * @Description: 
 * @FilePath: /MiniSQL/API/API.hpp
 */
#pragma once

#include "Interpreter.hpp"
#include "CatalogManager.hpp"
#include "Condition.hpp"
#include "BufferManager.hpp"

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
static BufferManager *bm;
BufferManager * getBufferManager();

} // namespace API