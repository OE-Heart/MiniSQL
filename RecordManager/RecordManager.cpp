/*
 * @Author: Yinwhe
 * @Date: 2021-06-16 09:50:16
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 12:22:57
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "RecordManager.hpp"

namespace RM{

void Rpanic(const char* s){
    printf("[RecordManager Panic] %s", s);
    while(1);
}

void createTable(Table &t){
    BID bid = bm->bread(t.tableName, 0);
    char* data = bm->baddr(bid);
    memset(data,0, BLOCK_SIZE);
    bm->bwrite(bid);
    bm->brelease(bid);
    t.blockCnt = 1;
    t.recordCnt = 0;
}

void dropTable(Table &t){
    bm->bflush(t.tableName);
    remove((t.tableName + ".data").c_str());
}

static ValueVec &getRecord(Table &t, char *data){
    const std::vector<Column> &attr = t.columns;
    std::vector<Value> res;
    int size = attr.size();
    data++; // skip the first sign byte
    for (int offset = 0, i = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT){
            double val = *(double *)(data + offset);
            // std::cerr << "DVAL " << val << std::endl;
            res.push_back(val);
        }
        else if (attr[i].field == Field::INT){
            int val = *(int *)(data + offset);
            // std::cerr << "IVAL " << val << std::endl;
            res.push_back(val);
        }
        else{ // string
            std::string val = std::string((char *)(data + offset));
            // std::cerr << "SVAL " << val << std::endl;
            res.push_back(val);
        }
        offset += attr[i].size();
    }
    return res;
}

static void putRecord(Table &t, const std::vector<Value> v, char *data){
    *data++ = 1; // set as taken
    const std::vector<Column> &attr = t.columns;
    int size = attr.size();
    for (int i = 0, offset = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT)
        {
            double val = std::get<double>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
        }
        else if (attr[i].field == Field::INT)
        {
            int val = std::get<int>(v[i]);
            memcpy(data + offset, &val, attr[i].size());
        }
        else // string
        {
            std::string val = std::get<std::string>(v[i]);
            memcpy(data + offset, val.c_str(), attr[i].size());
        }
        offset += attr[i].size();
    }
}

static bool checkUnique(Table &t, int ColumnID, Value &v){
    // if (t.columns[ColumnID].index != "")
    // { // use index to check uniqueness
    //     PieceVec vec = IndexSelect(t, t.attrbs[id], Condition(t.attrbs[id].name, v, CondType::EQUAL));
    //     return not vec.empty();
    // }
    int blockcount = t.blockCnt;
    for (int i = 0; i < blockcount; i++){
        int bid = bm->bread(t.tableName, i);
        char *data = bm->baddr(bid);
        int size = t.size()+1;
        for (int offset = 0; offset < BLOCK_SIZE; offset += size){
            if (data[offset] == 1){
                std::vector<Value> &vals = getRecord(t, data + offset);
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

Piece InsertRecord(Table &t, const std::vector<Value> &vals){
    // Check uniqueness first
    for (int i=0;i<t.columns.size();i++){
        if (t.columns[i].isUnique && !checkUnique(t, i, vals[i]))
            throw RecordError("Insert fails, attribute " + t.columns[i].columnName + " not unique!");
    }

    int blockcount = t.blockCnt;
    BID bid = bm->bread(t.tableName, blockcount - 1); // Trying to insert into the last page
    char *data = bm->baddr(bid);
    int size = t.size() + 1;

    for (int offset = 0; offset < BLOCK_SIZE; offset += size){
        if (data[offset] == 0){ // found free record
            putRecord(t, vals, data + offset);
            bm->brelease(bid);
            return std::make_pair(bid, offset);
        }
    }
    bm->brelease(bid);

    // Or we need to allocate a new page
    t.blockCnt += 1;
    bid = bm->bread(t.tableName, t.blockCnt -  1);
    data = bm->baddr(bid);
    putRecord(t, vals, data);
    bm->brelease(bid);

    return std::make_pair(bid, 0);
}

static PieceVec Intersect(PieceVec p, PieceVec q){
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

static PieceVec SelectPos(Table &t, const std::vector<Condition> con)
{
    PieceVec v;
    bool flag = false;
    // for (Condition c : con)
    // {
    //     if (t.getColumn(c.columnName).index != "")
    //     {
    //         if (!flag)
    //         {
    //             flag = true;
    //             v = IndexSelect(t, t.GetAttrb(c.attrb), c);
    //         }
    //         else
    //         {
    //             v = Intersect(v, IndexSelect(t, t.GetAttrb(c.attrb), c));
    //         }
    //     }
    // }
    if (!flag){ // Scan and search
        int blockcount = t.blockCnt;
        for (int i = 0; i < blockcount; i++){
            BID bid = bm->bread(t.tableName, i);
            char *data = bm->baddr(bid);
            int size = t.size()+1;
            for (int offset = 0; offset < BLOCK_SIZE; offset += size){
                if(!data[offset]) continue; // Empty skip
                
                std::vector<Value> res = getRecord(t, data + offset);
                bool good = true;
                for (Condition c : con){
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

void DeleteRecord(Table &t, const std::vector<Condition> con){
    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        data[piece.second] = 0;
        bm->bwrite(bid);
        bm->brelease(bid);
    }
}

std::vector<ValueVec> SelectRecord(Table &t, const std::vector<Condition> con)
{
    std::vector<std::vector<Value> > res;
    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        res.push_back(getRecord(t, data + piece.second));
        bm->brelease(bid);
    }
    return res;
}

} // namespace RM