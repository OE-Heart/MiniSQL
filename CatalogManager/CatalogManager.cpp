/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 17:41:38
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 10:50:49
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/CatalogManager.cpp
 */
#include "CatalogManager.hpp"

CatalogManager::CatalogManager()
{

}

CatalogManager::~CatalogManager()
{
    
}

void CatalogManager::newTable(const std::string &tableName, const std::vector<Column> &columns)
{
    tables[tableName] = Table{tableName, columns};
}

void CatalogManager::newIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName)
{
    indices[indexName] = Index{indexName, tableName, columnName};
    auto &table = tables[tableName];
    for (auto &column : table.columns) 
    {
        if (column.columnName == columnName) 
        {
            column.index = columnName;
            break;
        }
    }
}

void CatalogManager::dropTable(const std::string &tableName) 
{
    auto table = tables[tableName];
    tables.erase(tableName);
    for (auto column : table.columns) 
    {
        if (column.index != "") 
        {
            indices.erase(column.index);
        }
    }
}

void CatalogManager::dropIndex(const std::string &indexName) 
{
    auto index = indices[indexName];
    indices.erase(indexName);
    auto table = tables[index.tableName];
    for (auto &column : table.columns) 
    {
        if (column.columnName == index.columnName) 
        {
            column.index = "";
            break;
        }
    }
}

bool CatalogManager::existTable(const std::string &tableName) const
{
    return tables.count(tableName) == 1;
}

bool CatalogManager::existIndex(const std::string &indexName) const
{
    return indices.count(indexName) == 1;
}

bool CatalogManager::validName(const std::string &name) const 
{
    return tables.count(name) == 0 && indices.count(name) == 0;
}

Table CatalogManager::getTable(const std::string &tableName)
{
    return tables.at(tableName);
}

Index CatalogManager::getIndex(const std::string &indexName)
{
    return indices.at(indexName);
}

void CatalogManager::load()
{
    std::ifstream fin(catalogFile, std::ios::binary);
    if (!fin) return ;

    while (fin)
    {
        
    }
}

void CatalogManager::save()
{
    std::ofstream fout(catalogFile, std::ios::binary);

    std::string s;

    for (auto iter = tables.begin(); iter != tables.end(); iter++) 
    {
        s += char(1);
        std::string tableString = (iter->second).toString();
        int len = tableString.size();
        std::copy_n(reinterpret_cast<char *>(&len), sizeof(int), std::back_inserter(s));
        s + tableString;
    }
    
    for (auto iter = indices.begin(); iter != indices.end(); iter++)
    {
        s += char(0);
        Index index = iter->second;
        int indexNameSize = index.indexName.size();
        std::copy_n(reinterpret_cast<char *>(&indexNameSize), sizeof(int), std::back_inserter(s));
        std::copy_n(index.indexName.c_str(), indexNameSize, back_inserter(s));
        int tableNameSize = index.tableName.size();
        std::copy_n(reinterpret_cast<char *>(&tableNameSize), sizeof(int), std::back_inserter(s));
        std::copy_n(index.tableName.c_str(), tableNameSize, std::back_inserter(s));
        int columnNameSize = index.columnName.size();
        std::copy_n(reinterpret_cast<char *>(&columnNameSize), sizeof(int), std::back_inserter(s));
        std::copy_n(index.columnName.c_str(), columnNameSize, std::back_inserter(s));
    }

    fout.write(s.c_str(), s.size());
}