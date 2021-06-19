/*
 * @Author: Yinwhe
 * @Date: 2021-06-18 21:53:58
 * @LastEditTime: 2021-06-19 12:22:45
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


typedef std::pair<int, int> Piece;
typedef std::vector<Piece> PieceVec;
typedef std::vector<Value> ValueVec;

BufferManager *bm;
void Rpanic(const char *);
void CreateTable(Table &t);
void DropTable(Table &t);
Piece InsertRecord(Table &t, const std::vector<Value> &vals);
void DeleteRecord(Table &t, const std::vector<Condition> conds);
std::vector<ValueVec> SelectRecord(Table &t, const std::vector<Condition> conds);

}