/**
 * @file IndexManager.hpp
 * @author Gcx(adjust from NCJ)
 * @implementation of B+TreeNode
 * @version 0.1
 * @date 2021-06-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include <string>
#include <map>
#include <memory>
#include "BPTreeNode.hpp"
#include "BPTree.hpp"
#include "BufferManager.hpp"
#include "Table.hpp"

template<typename T>
class IndexManager;

template<typename T>
class IndexManager{
public:
    BPTree tree;
    IndexManager()=default;
    ~IndexManager();
    void create_index(BPTree &tree,Table & table,std::string & column_name);
    int find_index(BPTree &tree,const T &key);
    void insert_index(BPTree &tree,const T &key,int offset);
    void delete_index(BPTree &tree,const T &key,int offset);
    void drop_index(){
        BPTree<T>::cascadeDelete(Tree->root);
    }
};

template<typename T>
void create_index(BPTree &tree,Table & table,std::string & column_name){
    tree.filename=table.filename;
    tree.root = new BPTreeNode<T>::BPTreeNode<T>(degree, true);
    tree.keyCount = 0;
    tree.level = 1;
    tree.nodeCount = 1;
    tree.head = tree.root;
    tree.sizeofKey=sizeof(T);
    tree.degree= 4096/ (sizeof(T) + sizeof(int));
    tree.root=nullptr;
    tree.head=nullptr;
    int num_record=4096/(table.size()+1);//每个block里有这么多条record
    int index_col= Table::indexOfCol(column_name);
    for(int i=0;i<table.blockCnt;i++){//找blockcnt个block  
        BID blk=BufferManager::bread(table.tableName,i); //blk是table这个文件的第i个块
        char *data=BufferManager::baddr(blk);
        for(int j=0;j<num_record;j++){//每一条record
            if(*data==0) continue;
            else{
                data++;
                for(int k=0;k<index_col;k++){//找到column_name对应的那个地址起始值
                    data=data+Table::columns[index_col].size();
                }
                T key = *(T*)data;
                tree.BPTree::insert(key,i*num_record+j);
                data=data+table.size()+1;//下一条record
            }
        }
    }
}

template<typename T>
int find_index(BPTree &tree,const T &key){
    return tree.BPTree<T>::find(key); 
}

template<typename T>
void insert_index(BPTree &tree,const T &key,int offset){
    tree.BPTree<T>::insert(key,offset);
}

template<typename T>
void delete_index(BPTree &tree,const T &key,int offset){
    tree.BPTree<T>::remove(key,offset);
}