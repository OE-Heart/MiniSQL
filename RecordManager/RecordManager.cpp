/*
 * @Author: Yinwhe
 * @Date: 2021-06-16 09:50:16
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-20 16:54:22
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "RecordManager.hpp"
#include "IndexManager.hpp"

// #define DEBUG

BufferManager *bm;
IndexManager  *im;

RecordManager::RecordManager(){
    bm = new BufferManager();
    im = new IndexManager();
}

RecordManager::~RecordManager(){
    delete bm;
    delete im;
}

void RecordManager::Rpanic(const char* s){
    printf("[RecordManager Panic] %s", s);
    while(1);
}

void RecordManager::CreateTable(Table &t){
    BID bid = bm->bread(t.tableName, 0);
    char* data = bm->baddr(bid);
    memset(data,0, BLOCK_SIZE);
    bm->bwrite(bid);
    bm->brelease(bid);
    t.blockCnt = 1;
    t.recordCnt = 0;
    #ifdef DEBUG
    printf("RM CreateTable Done\n");
    #endif
}

void RecordManager::DropTable(Table &t){
    bm->bflush(t.tableName);
    remove((t.tableName + ".data").c_str());
    #ifdef DEBUG
    printf("RM DropTable Done\n");
    #endif
}

ValueVec RecordManager::GetRecord(Table &t, char *data){
    #ifdef DEBUG
    printf("RM GetRecord data: %p\n", data);
    #endif
    const std::vector<Column> &attr = t.columns;
    std::vector<Value> res;
    int size = attr.size();
    data++; // skip the first sign byte
    for (int offset = 0, i = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT){
            double val = *(double *)(data + offset);
            res.push_back(Value(val));
            #ifdef DEBUG
            printf("RM GetRecord Double %f\n", val);
            #endif
        }
        else if (attr[i].field == Field::INT){
            int val = *(int *)(data + offset);
            res.push_back(Value(val));
            #ifdef DEBUG
            printf("RM GetRecord Int %d\n", val);
            #endif
        }
        else{ // string
            std::string val = std::string((char *)(data + offset));
            res.push_back(Value(val));
            #ifdef DEBUG
            printf("RM GetRecord String %s\n", val.c_str());
            #endif
        }
        offset += attr[i].size();
    }
    return res;
}

void RecordManager::PutRecord(Table &t, const std::vector<Value> v, char *data){
    *data++ = 1; // set as taken
    const std::vector<Column> &attr = t.columns;
    int size = attr.size();
    for (int i = 0, offset = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT)
        {
            double val = std::get<double>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
            #ifdef DEBUG
            printf("RM PutRecord Double %f\n", val);
            #endif
        }
        else if (attr[i].field == Field::INT)
        {
            int val = std::get<int>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
            #ifdef DEBUG
            printf("RM PutRecord Int %d\n", val);
            #endif
        }
        else // string
        {
            std::string val = std::get<std::string>(v[i]);
            memcpy(data + offset, val.c_str(), attr[i].size());
            #ifdef DEBUG
            printf("RM PutRecord String %s\n", val.c_str());
            #endif
        }
        offset += attr[i].size();
    }
    t.recordCnt+=1;
}

bool RecordManager::CheckUnique(Table &t, int ColumnID, const Value &v){
    if (t.columns[ColumnID].index != "")
    { // use index to check uniqueness
        PieceVec vec = IndexSelect(t, ColumnID, Condition(t.columns[ColumnID].columnName, OP::EQ, v));
        return !vec.empty();
    }
    int blockcount = t.blockCnt;
    for (int i = 0; i < blockcount; i++){
        int bid = bm->bread(t.tableName, i);
        char *data = bm->baddr(bid);
        int size = t.size()+1;
        for (int offset = 0; offset < BLOCK_SIZE; offset += size){
            if (data[offset] == 1){
                std::vector<Value> vals = GetRecord(t, data + offset);
                if (vals[ColumnID] == v){
                    bm->brelease(bid);
                    return false;
                }
            }
        }
        bm->brelease(bid);
    }
    return true;
}

bool RecordManager::CheckType(Column &col, const Value &v){
    switch(col.field){
        case Field::INT:    return (std::get_if<int>(&v) != nullptr);
        case Field::FLOAT:  return (std::get_if<double>(&v) != nullptr);
        case Field::CHAR:   return (std::get_if<std::string>(&v) != nullptr);
    }
}

Piece RecordManager::InsertRecord(Table &t, const std::vector<Value> &vals){
    // Check Type
    auto &attr=t.columns;
    for(int i=0;i<attr.size();i++){
        if(!CheckType(attr[i], vals[i])){
            printf("Insert fails, attribute %s type error!\n", attr[i].columnName.c_str());
            return std::make_pair(-1, -1);
        }       
    }

    // Check uniqueness
    for (int i=0;i<attr.size();i++){
        if (attr[i].isUnique && !CheckUnique(t, i, vals[i])){
            printf("Insert fails, attribute %s not unique!", attr[i].columnName.c_str());
            return std::make_pair(-1, -1);
        }
    }
    
    int blockcount = t.blockCnt;
    BID bid = bm->bread(t.tableName, blockcount - 1); // Trying to insert into the last page
    char *data = bm->baddr(bid);
    int size = t.size() + 1;

    for (int offset=0;offset<BLOCK_SIZE;offset+=size){
        if (data[offset] == 0 && offset+size<BLOCK_SIZE){ // found free record
            PutRecord(t, vals, data+offset);
            bm->bwrite(bid);
            bm->brelease(bid);
            #ifdef DEBUG
            printf("InsertRecord Success, bid:%d, off:%d\n", bid, offset);
            #endif
            return std::make_pair(bid, offset);
        }
    }
    bm->brelease(bid);

    // Or we need to allocate a new page
    t.blockCnt += 1;
    bid = bm->bread(t.tableName, t.blockCnt-1);
    data = bm->baddr(bid);
    #ifdef DEBUG
    printf("InsertRecord, new block created:%d\n", bid);
    #endif
    PutRecord(t, vals, data);
    bm->bwrite(bid);
    bm->brelease(bid);

    return std::make_pair(bid, 0);
}

PieceVec RecordManager::Intersect(PieceVec p, PieceVec q){
    std::sort(p.begin(), p.end());
    std::sort(q.begin(), q.end());
    PieceVec::iterator ip = p.begin(), iq = q.begin();
    PieceVec res;
    while (ip != p.end() && iq != q.end())
    {
        while (ip != p.end() && *ip < *iq)
        {
            ++ip;
        }
        while (iq != q.end() && *iq < *ip)
        {
            ++iq;
        }
        while (ip != p.end() && iq != q.end() && *ip == *iq)
        {
            res.push_back(*ip);
            ++ip;
            ++iq;
        }
    }
    return res;
}

PieceVec RecordManager::SelectPos(Table &t, const std::vector<Condition> con)
{
    PieceVec v;
    bool flag = false;
    for (const Condition &c : con){
        if(c.op != OP::EQ) break; // Not supported
        int index = t.indexOfCol(c.columnName);
        if (t.columns[index].index != ""){
            if (!flag)
            {
                flag = true;
                v = IndexSelect(t, index, c);
            }
            else
            {
                v = Intersect(v, IndexSelect(t, t.indexOfCol(c.columnName), c));
            }
        }
    }
    if (!flag){ // Scan and search
        int blockcount = t.blockCnt;
        for (int i = 0; i < blockcount; i++){
            BID bid = bm->bread(t.tableName, i);
            char *data = bm->baddr(bid);
            int size = t.size()+1;
            for (int offset = 0; offset < BLOCK_SIZE; offset += size){
                if(!data[offset]) continue; // Empty skip
                
                std::vector<Value> res = GetRecord(t, data + offset);
                bool good = true;
                for (auto const &c : con){
                    int index = t.indexOfCol(c.columnName);
                    if (!c.isTrue(res[index])){
                        good = false;
                        break;
                    }
                }
                if (good)
                    v.push_back(std::make_pair(i, offset));
            }
            bm->brelease(bid);
        }
    }
    return v;
}

void RecordManager::DeleteRecord(Table &t, const std::vector<Condition> &con){
    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        data[piece.second] = 0;
        t.recordCnt-=1;
        bm->bwrite(bid);
        bm->brelease(bid);
    }
}

void RecordManager::DeleteAllRecord(Table &t){
    std::vector<Condition> con; // Con is empty
    DeleteRecord(t, con);
}

std::vector<ValueVec> RecordManager::SelectRecord(Table &t, const std::vector<Condition> &con)
{
    std::vector<std::vector<Value> > res;

    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        #ifdef DEBUG
        printf("SelectRecord firsr:%d, second:%d\n", piece.first, piece.second);
        #endif
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        res.push_back(GetRecord(t, data + piece.second));
        bm->brelease(bid);
    }
    return res;
}

std::vector<ValueVec> RecordManager::SelectAllRecord(Table &t){
    std::vector<Condition> con; // Con is empty
    return SelectRecord(t, con);
}


PieceVec RecordManager::IndexSelect(Table &t, int ColumnID, const Condition &con){
    const auto &attr = t.columns[ColumnID];
    PieceVec res;
    if(attr.columnName != con.columnName)
        Rpanic("IndexSelect error, column name inconsistent!");
    
    if(con.op != OP::EQ)
        Rpanic("IndexSelect error, range select not supported!");

    int off = im->FindIndex(attr.index, t, attr.columnName, con.value);
    res.emplace_back(std::make_pair(off/BLOCK_SIZE, off%BLOCK_SIZE));
    return res;
}