/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 17:42:22
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 20:49:24
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/CatalogManager.hpp
 */
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Table.hpp"
#include "Column.hpp"
#include "Index.hpp"

class CatalogManager{
public:
    CatalogManager();
    ~CatalogManager();

    void newTable(const std::string &tableName, const std::vector<Column> &columns);
    void newIndex(const std::string &indexName, const std::string &tableName, const std::string &columnName);
    void dropTable(const std::string &tableName);
    void dropIndex(const std::string &indexName);
    bool validName(const std::string &name) const;
    
    void load();
    void save();

private:
    std::unordered_map<std::string, Table> tables;
    std::unordered_map<std::string, Index> indices;
};