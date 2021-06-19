/**
 * @file BPTreeNode.hpp
 * @author Gcx(adjust from NCJ)
 * @implementation of B+TreeNode
 * @version 0.1
 * @date 2021-06-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
class BPTreeNode;

template<typename T>
class BPTreeNode{
public:
	bool isLeaf;//是否为叶子结点
    int degree;//结点的度数
    int cnt;//结点中当前的key的数量
    BPTreeNode *parent, *sibling;
    vector<T> keys;//叶子和非叶子结点都有的搜索码的值 
    vector<int> keyOffset;//叶子结点的搜索码对应的记录的序号
    vector<BPTreeNode<T> *> children;//非叶子结点的子结点
	BPTreeNode() = default;
    BPTreeNode(int degree, bool isLeaf);
	~BPTreeNode() {}
	bool search(const T &key, int &index) const;
    BPTreeNode *split(T &key);
    int add(const T &key);
    int add(const T &key, int offset);
    void removeAt(int index);
    bool isRoot() const { return parent == nullptr; }
    bool binarySearch(const T &key, int &index) const;
};

template<typename T>
BPTreeNode<T>::BPTreeNode(int degree, bool isLeaf) : degree(degree), isLeaf(isLeaf), cnt(0), parent(nullptr),
                                                     sibling(nullptr) {
    //resize的作用是分配最大容量并同时创建对象                                                    
    children.resize(degree + 1);//children数量为度数+1 
    keys.resize(degree);//搜索码的数量为度数 
    keyOffset.resize(degree);//跟搜索码数量相同 
}

template<typename T>//index的意思是这个key应该放在这个Node里的第几个位置上
bool BPTreeNode<T>::search(const T &key, int &index) const {
    if (cnt == 0) {//结点目前没有key，它的位置该是0
        index = 0;
        return false;
    }
    if (key < keys[0] ) {//比keys[0]还小，那位置该是0
        index = 0;
        return false;
    }
    if (key > keys[cnt - 1]){ //比keys[cnt-1]（目前有的最后一个）还大，那位置该是cnt
        index = cnt;
        return false;
    }
    return binarySearch(key, index);
}

template<typename T>
bool BPTreeNode<T>::binarySearch(const T &key, int &index) const {
    int left = 0, right = cnt - 1, pos;
    while (left <= right) {
        pos = left + (right - left) / 2;
        if (keys[pos] < key) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    index = left;
    return keys[index] == key;
}

template<typename T>
BPTreeNode<T> *BPTreeNode<T>::split(T &key) {
    //key用来往上层传
    BPTreeNode<T> *newNode = new BPTreeNode<T>(degree, isLeaf);
    int minimal = (degree - 1) / 2;//[n/2]-1
    if (isLeaf) {//叶子结点分keys,叶子的元素数量为[n/2]~n-1
        key = keys[minimal + 1];
        //把从[n/2]开始的key赋值给新的
        for (int i = minimal + 1; i < degree; i++) {
            newNode->keys[i - minimal - 1] = keys[i];
            newNode->keyOffset[i - minimal - 1] = keyOffset[i];
        }
        newNode->sibling = this->sibling;
        this->sibling = newNode;
        this->cnt = minimal + 1;
    }
    else {//非叶子结点分children和keys，非叶子的元素数量为[n/2]~n
        key = keys[minimal];
        for (int i = minimal + 1; i <= degree; i++) {
            newNode->children[i - minimal - 1] = this->children[i];
            this->children[i]->parent = newNode;
            this->children[i] = nullptr;
        }
        for (int i = minimal + 1; i < degree; i++) {
            newNode->keys[i - minimal - 1] = keys[i];
        }
        this->cnt = minimal;
    }
    newNode->parent = this->parent;
    newNode->cnt = degree - minimal - 1;
    return newNode;
}

template<typename T>
int BPTreeNode<T>::add(const T &key) {//非叶子结点增加记录
    int index;
    bool keyExists = search(key, index);
    if (keyExists) {
        cerr << "Key is not unique!" << endl;
        exit(10);
    }
    for (int i = cnt; i > index; i--) {
        keys[i] = keys[i - 1];
        children[i + 1] = children[i];
    }
    keys[index] = key;//在index处增加这条记录
    children[index + 1] = nullptr;
    cnt++;
    return index;
}
template<typename T>
int BPTreeNode<T>::add(const T &key, int offset) {//叶子结点增加记录
    int index;
    bool keyExists = search(key, index);
    if (keyExists) {
        cerr << "Key is not unique!" << endl;
        exit(10);
    }
    for (int i = cnt; i > index; i--) {
        keys[i] = keys[i - 1];
        keyOffset[i] = keyOffset[i - 1];
    }
    keys[index] = key;
    keyOffset[index] = offset;
    cnt++;
    return index;
}

template<typename T>
void BPTreeNode<T>::removeAt(int index) {//删除node中第index的key和children
    for (int i = index; i < cnt - 1; i++) {
        keys[i] = keys[i + 1];
    }
    if (isLeaf) {//叶子结点
        for (int i = index; i < cnt - 1; i++) {
            keyOffset[i] = keyOffset[i + 1];
        }
        keyOffset[cnt - 1] = 0;
        keys[cnt - 1] = T();//多出来的空位
    }
    else {//非叶子结点
        for (int i = index + 1; i < cnt; i++) {
            children[i] = children[i + 1];
        }
        keys[cnt - 1] = T();//多出来的空位
        children[cnt] = nullptr;//多出来的空位
    }
    cnt--;
}
