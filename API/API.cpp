/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:37
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 14:37:38
 * @Description: 
 * @FilePath: /MiniSQL/API/API.cpp
 */
#include "API.hpp"

MiniSQL::MiniSQL()
{
    
}

MiniSQL::~MiniSQL()
{

}

void API::createTable(const std::string &tableName, const std::vector<Column> &columns, const std::string &primaryKey)
{
    auto CM = API::getCatalogManager();
    auto RM = API::getRecordManager();

    if (CM->existTable(tableName))
    {
        std::cout << "ERROR : You have an error in your SQL syntax; tabel named " << tableName << " already exists.\n";
        return ;
    }

    CM->newTable(tableName, columns);
    Table &table = CM->getTable(tableName);
    RM->CreateTable(table);
}

void API::createIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName)
{
    
}

void API::dropTable(const std::string &tableName)
{    
    auto CM = API::getCatalogManager();
    auto RM = API::getRecordManager();

    if (CM->existTable(tableName))
    {
        std::cout << "ERROR : You have an error in your SQL syntax; tabel named " << tableName << " doesn't exist.\n";
        return ;
    }

    Table &table = CM->getTable(tableName);
    RM->DropTable(table);
    CM->dropTable(tableName);
}

void API::dropIndex(const std::string &indexName)
{

}

void API::insertOn(const std::string &tableName, std::vector<Value> &valueList)
{

}

void API::deleteFrom(const std::string &tableName, std::vector<Condition> &conditionList)
{
    
}


void API::select(const std::string &tableName, std::vector<Condition> &conditionList)
{
    
}

CatalogManager *API::getCatalogManager()
{
    if (cm == NULL) cm = new CatalogManager();
    return cm;
}

RecordManager *API::getRecordManager()
{
    if (rm == NULL) rm = new RecordManager();
    return rm;
}