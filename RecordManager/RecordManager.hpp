/*
 * @Author: Yinwhe
 * @Date: 2021-06-18 21:53:58
 * @LastEditTime: 2021-06-19 10:58:03
 * @LastEditors: Yinwhe
 * @Description: Doc
 * @FilePath: /MiniSQL/RecordManager/RecordManager.hpp
 */
#pragma once
#include <string>
#include <vector>
#include <variant>
#include "buffermanager.hpp"
#include "Table.hpp"
#include "Condition.hpp"

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