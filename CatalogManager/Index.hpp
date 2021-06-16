/*
 * @Author: Ou Yixin
 * @Date: 2021-06-14 20:11:44
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-14 20:12:44
 * @Description: 
 * @FilePath: /MiniSQL/CatalogManager/Index.hpp
 */
#pragma once

#include <string>

struct Index
{
    std::string indexName;
    std::string tableName;
    std::string columnName;
};
