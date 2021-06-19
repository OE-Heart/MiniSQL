/*
 * @Author: Yinwhe
 * @Date: 2021-06-16 09:50:16
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 15:03:49
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "RecordManager.hpp"

RecordManager::RecordManager(){
    bm = new BufferManager();
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

ValueVec& RecordManager::GetRecord(Table &t, char *data){
    const std::vector<Column> &attr = t.columns;
    std::vector<Value> res;
    int size = attr.size();
    data++; // skip the first sign byte
    for (int offset = 0, i = 0; i < size; i++){
        if (attr[i].field == Field::FLOAT){
            double val = *(double *)(data + offset);
            res.push_back(val);
            #ifdef DEBUG
            printf("RM GetRecord Double %f\n", val);
            #endif
        }
        else if (attr[i].field == Field::INT){
            int val = *(int *)(data + offset);
            res.push_back(val);
            #ifdef DEBUG
            printf("RM GetRecord Int %d\n", val);
            #endif
        }
        else{ // string
            std::string val = std::string((char *)(data + offset));
            res.push_back(val);
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
}

bool RecordManager::CheckUnique(Table &t, int ColumnID, const Value &v){
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
                std::vector<Value> &vals = GetRecord(t, data + offset);
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

Piece RecordManager::InsertRecord(Table &t, const std::vector<Value> &vals){
    // Check uniqueness first
    for (int i=0;i<t.columns.size();i++){
        if (t.columns[i].isUnique && !CheckUnique(t, i, vals[i]))
            printf("Insert fails, attribute %s not unique!", t.columns[i].columnName.c_str());
    }

    int blockcount = t.blockCnt;
    BID bid = bm->bread(t.tableName, blockcount - 1); // Trying to insert into the last page
    char *data = bm->baddr(bid);
    int size = t.size() + 1;

    for (int offset = 0; offset < BLOCK_SIZE; offset += size){
        if (data[offset] == 0){ // found free record
            PutRecord(t, vals, data + offset);
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
    bid = bm->bread(t.tableName, t.blockCnt -  1);
    data = bm->baddr(bid);
    #ifdef DEBUG
    printf("InsertRecord, new block created:%d\n", bid);
    #endif
    PutRecord(t, vals, data);
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
                
                std::vector<Value> res = GetRecord(t, data + offset);
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

void RecordManager::DeleteRecord(Table &t, const std::vector<Condition> con){
    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        data[piece.second] = 0;
        bm->bwrite(bid);
        bm->brelease(bid);
    }
}

std::vector<ValueVec> RecordManager::SelectRecord(Table &t, const std::vector<Condition> con)
{
    std::vector<std::vector<Value> > res;
    PieceVec v = SelectPos(t, con);
    for (auto piece : v){
        BID bid = bm->bread(t.tableName, piece.first);
        char *data = bm->baddr(bid);
        res.push_back(GetRecord(t, data + piece.second));
        bm->brelease(bid);
    }
    return res;
}