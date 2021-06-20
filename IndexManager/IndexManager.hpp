/*
 * @Author: Gcx
 * @Date: 2021-06-19 18:28:23
 * @LastEditors: GCX
 * @LastEditTime: 2021-06-19 22:44
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */

#pragma once
#include <string>
#include <map>
#include <memory>
#include "BPTreeNode.hpp"
#include "BPTree.hpp"
#include "BufferManager.hpp"
#include "Table.hpp"

extern BufferManager* bm;

template<typename T>
class IndexManager;

template<typename T>
class IndexManager{
public:
    std::map<std::string,BPTree<T>> tree_map;
    //BPTree<T> tree;
    IndexManager()=default;
    ~IndexManager();
    void create_index(map<std::string,BPTree<T>> &tree_map,Table & table,std::string & column_name);
    int  find_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key);
    void insert_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key,int offset);
    void delete_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key,int offset);
    void alter_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key_before,const T &key_after,int offset);
    void drop_index(map<std::string,BPTree<T>> &tree_map,Table & table);
};

template<typename T>
void IndexManager<T>::create_index(map<std::string,BPTree<T>> &tree_map,Table & table,std::string & column_name){
    BPTree<T> tree;
    tree.filename=table.tableName;
    tree.keyCount = 0;
    tree.level = 1;
    tree.nodeCount = 1;
    tree.sizeofKey=sizeof(T);
    tree.degree= 4096/ (sizeof(T) + sizeof(int));
    tree.root = new BPTreeNode<T>::BPTreeNode<T>(tree.degree, true);
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
    std::pair<std::map<std::string,BPTree<T>>::iterator,bool> ret;
    ret=tree_map.insert(std::pair<std::string,BPTree<T>>(table.tableName,tree));
    if(ret.second==false){
        std::cout<<"This table has already had an index";
    }

}

template<typename T>
int  IndexManager<T>::find_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key){
    std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(table.tableName);
    BPTree<T> tree=ret.second;
    return tree.find(key); 
}

template<typename T>
void IndexManager<T>::insert_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key,int offset){
    std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(table.tableName);
    BPTree<T> tree=ret.second;
    tree.insert(key,offset);
}

template<typename T>
void IndexManager<T>::delete_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key,int offset){
    std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(table.tableName);
    BPTree<T> tree=ret.second;
    tree.remove(key);
}

template<typename T>
void IndexManager<T>::alter_index(map<std::string,BPTree<T>> &tree_map,Table & table,const T &key_before,const T &key_after,int offset){
    std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(table.tableName);
    BPTree<T> tree=ret.second;
    tree.remove(key_before);
    tree.insert(key_after,offset);    
}

template<typename T>
void drop_index(map<std::string,BPTree<T>> &tree_map,Table & table){
    std::map<std::string,BPTree<T>>::iterator ret;
    ret=tree_map.find(table.tableName);
    BPTree<T> tree=ret.second;
    BPTree<T>::cascadeDelete(tree->root);
    tree_map.erase(ret);
}