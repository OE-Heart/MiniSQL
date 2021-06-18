/**
 * @file RecordManager.hpp
 * @author Yinwhe
 * @brief Definition of the RecordManager
 * @version 0.1
 * @date 2021-06-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <string>
#include <vector>
#include <variant>
#include "buffermanager.hpp"
#include "Table.hpp"
#include "Column.hpp"

namespace RM{
    class RecordError : public std::exception{
        std::string msg;
    public:
        RecordError(const std::string info){
            msg = info;
        }
        const char *what() const noexcept override{
            return msg.c_str();
        }
    };
    BufferManager *bm;
    void Rpanic(const char *);
    void CreateTable(Table &t);
    void DropTable(Table &t);
    std::pair<int, int> InsertRecord(Table &t, const std::vector<Value> &vals);
    void DeleteRecord(Table &t, const std::vector<Condition> conds);
    std::vector<std::vector<Value>> SelectRecord(Table &t, const std::vector<Condition> conds);
}