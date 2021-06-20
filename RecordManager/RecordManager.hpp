/*
 * @Author: Yinwhe
 * @Date: 2021-06-18 21:53:58
 * @LastEditTime: 2021-06-20 16:52:28
 * @LastEditors: Yinwhe
 * @Description: Doc
 * @FilePath: /MiniSQL/RecordManager/RecordManager.hpp
 */
#pragma once
#include <string>
#include <vector>
#include <variant>
#include "BufferManager.hpp"
#include "Table.hpp"
#include "Condition.hpp"
#include "IndexManager.hpp"

typedef std::pair<int, int> Piece;
typedef std::vector<Piece> PieceVec;
typedef std::vector<Value> ValueVec;

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

class RecordManager{
private:
    void        Rpanic(const char *);
    ValueVec    GetRecord(Table &, char *);
    void        PutRecord(Table &, const std::vector<Value>, char *);
    bool        CheckUnique(Table &, int, const Value &);
    bool        CheckType(Column &, const Value &);
    PieceVec    Intersect(PieceVec , PieceVec );
    PieceVec    SelectPos(Table &, const std::vector<Condition>);
    PieceVec    IndexSelect(Table &t, int ColumnID, const Condition &con);
public:

    RecordManager();
    ~RecordManager();
    void    CreateTable(Table &t);
    void    DropTable(Table &t);
    Piece   InsertRecord(Table &t, const std::vector<Value> &vals);
    void    DeleteRecord(Table &t, const std::vector<Condition> &con);
    void    DeleteAllRecord(Table &t);
    std::vector<ValueVec> SelectRecord(Table &t, const std::vector<Condition> &con);
    std::vector<ValueVec> SelectAllRecord(Table &t);
    IndexManager  *im;
};