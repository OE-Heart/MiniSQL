/**
 * @file RecordManager.cpp
 * @author Yinwhe
 * @brief Implementation of RecordManager
 * @version 0.1
 * @date 2021-06-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <cstdio>
#include "RecordManager.hpp"
namespace RM{
    void Rpanic(const char* s){
        printf("[RecordManager Panic] %s", s);
        while(1);
    }
    void createTable(Table &t){
        BID bid = bm->bread(t.tableName, 0);
        char* data = bm->baddr(bid);
        data[0] = 0;    // Notify it as empty
        t.blockCnt = 1;
        t.recordCnt = 0;
    }
    
    void dropTable(Table &t){
        bm->bflush(t.tableName);
        remove((t.tableName + ".data").c_str());
    }

    static bool checkUnique(Table &t, int ColumnID, Value &v){
        
    }

    static void getRecord(){

    }

    static void putRecord(){

    }

    std::pair<int, int> InsertRecord(Table &t, const std::vector<Value> &vals){
        for (int i=0;i<t.columns.size();i++){
            if (t.columns[i].isUnique && !checkUnique(t, i, vals[i]))
                throw RecordError("Insert fails, attribute " + t.columns[i].columnName + " not unique!");
        }

        int blockcount = t.blockCnt;
        BID bid = bm->bread(t.tableName, blockcount - 1); // Trying to insert into the last page!
        char *data = bm->baddr(bid);

        for (int offset = 0; offset < BLOCK_SIZE; offset += t.size())
        {
            if (data[offset] == 0) // found empty piece
            {
                PutTuple(t, vals, p + offset);
                return std::make_pair(pageid, offset);
            }
        }

        // need to create a new page
        t.n_blocks += 1;
        int newpageid = bm->getPageId(t.name, blockcount);
        char *np = bm->getPageAddress(newpageid);
        PutTuple(t, vals, np);
        return std::make_pair(newpageid, 0);
    }
}