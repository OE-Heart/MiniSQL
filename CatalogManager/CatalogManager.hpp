/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 17:42:22
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-19 15:32:26
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/CatalogManager.hpp
 */
#pragma once

#include <iostream>
#include <fstream>
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
    bool existTable(const std::string &tableName) const;
    bool existIndex(const std::string &indexName) const;
    bool validName(const std::string &name) const;

    Table &getTable(const std::string &tableName);
    Index &getIndex(const std::string &indexName);
    
    void load();
    void save();

    std::unordered_map<std::string, Index> indices;

private:
    std::unordered_map<std::string, Table> tables;
    inline static const std::string catalogFile = "Catalog.minisql";
};