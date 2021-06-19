/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:26
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 17:23:42
 * @Description: 
 * @FilePath: /MiniSQL/API/API.hpp
 */
#pragma once

#include <iomanip>
#include "Interpreter.hpp"
#include "CatalogManager.hpp"
#include "Condition.hpp"
#include "BufferManager.hpp"
#include "RecordManager.hpp"

class MiniSQL{
public:
    MiniSQL();
    ~MiniSQL();
    Interpreter interpreter;
private:
};

namespace API{

void createTable(const std::string &tableName, const std::vector<Column> &columns, const std::string &primaryKey);
void createIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName);
void dropTable(const std::string &tableName);
void dropIndex(const std::string &indexName);
void insertOn(const std::string &tableName, std::vector<Value> &valueList);
void deleteFrom(const std::string &tableName, std::vector<Condition> &conditionList);
void select(const std::string &tableName, std::vector<Condition> &conditionList);

static CatalogManager *cm;
CatalogManager *getCatalogManager();
static RecordManager *rm;
RecordManager *getRecordManager();


} // namespace API
