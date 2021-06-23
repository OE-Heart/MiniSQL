/*
 * @Author: Yinwhe
 * @Date: 2021-06-18 21:53:58
 * @LastEditTime: 2021-06-23 16:47:27
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

class RecordManager{
private:
    void        Rpanic(const char *);
    ValueVec    GetRecord(Table &, char *);
    void        PutRecord(Table &, const ValueVec &, char *);
    bool        CheckUnique(Table &, int, const Value &);
    bool        CheckType(Column &, const Value &);
    PieceVec    Intersect(PieceVec , PieceVec );
    PieceVec    SelectPos(Table &, const std::vector<Condition>);
    PieceVec    IndexSelect(Table &t, int ColumnID, const Condition &con);
    void        IndexUpdate(Table &t, const ValueVec &vals, int optype, int offset);
public:

    RecordManager();
    ~RecordManager();
    void    CreateTable(Table &t);
    void    DropTable(Table &t);
    Piece   InsertRecord(Table &t, const ValueVec &vals);
    void    DeleteRecord(Table &t, const std::vector<Condition> &con);
    void    DeleteAllRecord(Table &t);
    std::vector<ValueVec> SelectRecord(Table &t, const std::vector<Condition> &con);
    std::vector<ValueVec> SelectAllRecord(Table &t);
    IndexManager  *im;
};