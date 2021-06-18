/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:37
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-18 15:48:37
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

bool API::createIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName)
{
    
    return true;
}

bool API::dropTable(const std::string &tableName)
{
    
    return true;
}

bool API::dropIndex(const std::string &indexName)
{

    return true;
}

bool API::insertOn(const std::string &tableName, std::vector<Value> &valueList)
{

}

bool select(const std::string &tableName, std::vector<Condition> &conditionList)
{
    
}

CatalogManager *API::getCatalogManager()
{
    if (cm == NULL) cm = new CatalogManager();
    return cm;
}