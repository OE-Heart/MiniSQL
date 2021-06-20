/*
 * @Author: Gcx
 * @Date: 2021-06-19 18:28:23
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-20 16:55:20
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */

#pragma once
#include <string>
#include <map>
#include "BPTreeNode.hpp"
#include "BPTree.hpp"
#include "BufferManager.hpp"
#include "Table.hpp"
#include "Condition.hpp"

extern BufferManager* bm;

template<typename T>
class _index_manager{
public:
    std::map<std::string, BPTree<T>> tree_map;
    _index_manager(){};
    ~_index_manager(){};
    void create_index(const std::string index_name, Table & table,const std::string & column_name);
    int  find_index(const std::string index_name, Table & table,const T &key);
    void insert_index(const std::string index_name, Table & table,const T &key,int offset);
    void delete_index(const std::string index_name, Table & table,const T &key);
    void alter_index(const std::string index_name, Table & table,const T &key_before,const T &key_after,int offset);
    void drop_index(const std::string index_name, Table & table);
};

template<typename T>
void _index_manager<T>::create_index(const std::string index_name, Table & table,const std::string & column_name){
    BPTree<T> tree(index_name, sizeof(T), 4096/(sizeof(T) + sizeof(int)));

    std::pair<typename std::map<std::string, BPTree<T>>::iterator, bool> ret;
    ret=tree_map.insert(std::pair<std::string, BPTree<T>>(index_name, tree));
    if(ret.second==false){
        std::cout<<"This table already had an index";
        return;
    }

    tree.keyCount = 0;
    tree.level = 1;
    tree.nodeCount = 1;
    tree.root = new BPTreeNode<T>(tree.degree, true);
    tree.head = tree.root;

    int num_record=4096/(table.size()+1);//每个block里有这么多条record
    int index_col= table.indexOfCol(column_name);
    for(int i=0;i<table.blockCnt;i++){//找blockcnt个block  
        BID blk=bm->bread(table.tableName,i); //blk是table这个文件的第i个块
        char *data=bm->baddr(blk);
        for(int j=0;j<num_record;j++){//每一条record       
            if(*data==0){
                data+=table.size()+1;//下一条record
                continue;
            } 
            else{
                int sum=0;
                sum++;
                for(int k=0;k<index_col;k++){//找到column_name对应的那个地址起始值
                    sum+=table.columns[k].size();
                }
                data+=sum;
                T key = *(T*)data;
                tree.insert(key,i*num_record+j);   
                data=data+table.size()+1-sum;//下一条record             
            }
        }
    }
}

template<typename T>
int _index_manager<T>::find_index(const std::string index_name, Table & table,const T &key){
    typename std::map<std::string, BPTree<T>>::iterator ret;
    ret=tree_map.find(index_name);
    BPTree<T> tree=ret->second;
    return tree.find(key);
}

template<typename T>
void _index_manager<T>::insert_index(const std::string index_name, Table & table,const T &key,int offset){
    typename std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(index_name);
    BPTree<T> tree=ret->second;
    tree.insert(key,offset);
}

template<typename T>
void _index_manager<T>::delete_index(const std::string index_name, Table & table,const T &key){
    typename std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(index_name);
    BPTree<T> tree=ret->second;
    tree.remove(key);
}

template<typename T>
void _index_manager<T>::alter_index(const std::string index_name, Table & table,const T &key_before,const T &key_after,int offset){
    typename std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(index_name);
    BPTree<T> tree=ret->second;
    tree.remove(key_before);
    tree.insert(key_after,offset);    
}

template<typename T>
void _index_manager<T>::drop_index(const std::string index_name, Table & table){
    typename std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(index_name);
    BPTree<T> tree=ret->second;
    tree.cascadeDelete(tree.root);
    tree_map.erase(ret);
}

class IndexManager{
private:
    _index_manager<int> *int_im;
    _index_manager<double> *float_im;
    _index_manager<std::string> *char_im;
public:
    IndexManager();
    ~IndexManager();
    void CreateIndex(const std::string index_name, Table & table, const std::string & column_name);
    int  FindIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val);
    void InsertIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val, int offset);
    void DeleteIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val);
    void AlterIndex(const std::string index_name, Table & table, const std::string & column_name, const Value &val_before, const Value &val_after, int offset);
    void DropIndex(const std::string index_name, Table & table, const std::string & column_name);
};