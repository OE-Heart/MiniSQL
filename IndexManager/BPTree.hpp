/**
 * @file BPTree.hpp
 * @author Gcx(adjust from NCJ)
 * @implementation of B+Tree
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
#include "BPTreeNode.hpp"

using namespace std;

template<typename T>
struct Nodemap {
    int index;
    BPTreeNode<T> *node;
};


template<typename T>
class BPTree {
public:
    typedef BPTreeNode<T> *TreeNode;
    string fileName;
    TreeNode root, head;
    int sizeofKey, level, keyCount, nodeCount, degree;
    BPTree(string fileName, int sizeofKey, int degree);
    ~BPTree(){}
    TreeNode getHeadNode() const { return head; }
    int find(const T &key);
    Nodemap<T> findNode(const T &key);
    bool insert(const T &key, int offset);
    bool remove(const T &key);
    void initBPTree();
    bool findKeyFromNode(TreeNode node, const T &key, Nodemap<T> &res);
    void cascadeInsert(TreeNode node);
    bool cascadeDelete(TreeNode node);
    bool deleteBranchLL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchLR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchRL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteBranchRR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafLL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafLR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafRL(TreeNode node, TreeNode parent, TreeNode sibling, int index);
    bool deleteLeafRR(TreeNode node, TreeNode parent, TreeNode sibling, int index);
};

template<typename T>
BPTree<T>::BPTree(string fileName, int sizeofKey, int degree) : fileName(fileName), sizeofKey(sizeofKey),
                                                                degree(degree), keyCount(0), nodeCount(0), level(0),
                                                                root(nullptr), head(nullptr) {
    initBPTree();
}

template<typename T>
void BPTree<T>::initBPTree() {
    root = new BPTreeNode<T>(degree, true);
    keyCount = 0;
    level = 1;
    nodeCount = 1;
    head = root;
}

template<typename T>
bool BPTree<T>::findKeyFromNode(TreeNode node, const T &key, Nodemap<T> &res) {
    int index;
    if (node->search(key, index)) {//在这个node里找到了key
        if (node->isLeaf) {//这个node是叶子
            res.index = index;
        }
		else {//这个node不是叶子
            node = node->children[index + 1];//往下一层之后，一定会是最左边那个
            while (!node->isLeaf) { node = node->children[0]; }
            res.index = 0;
        }
        res.node = node;
        return true;
    }
	else {//node里没找着这个key
        if (node->isLeaf) {
            res.node = node;
            res.index = index;
            return false;//没有
        }
		else{
            return findKeyFromNode(node->children[index], key, res);
        }
    }
}

template<typename T>
int BPTree<T>::find(const T &key) {//返回的是key所对应的keyoffset
    Nodemap<T> res;
    if (root==nullptr) { std::cout<<"root null\n"; return -1; }
    if (findKeyFromNode(root, key, res)) { return res.node->keyOffset[res.index]; }
    else { std::cout<<"not found\n"; return -1; }
}

template<typename T>
Nodemap<T> BPTree<T>::findNode(const T &key) {//返回key所在的node
    Nodemap<T> res;
    if (root==nullptr) { return res; }
    if (findKeyFromNode(root, key, res)) { return res; }
    else { return res; }
}

template<typename T>
bool BPTree<T>::insert(const T &key, int offset) {//往里加，度数在这个函数里判断
    Nodemap<T> res;
    if (root==nullptr) { initBPTree(); }
    if (findKeyFromNode(root, key, res)) {
        cerr << "Insert duplicate key!" << endl;
        return false;
    }
    res.node->add(key, offset);
    if (res.node->cnt == degree) {//到达度数时需要分裂
        cascadeInsert(res.node);
    }
    keyCount++;
    return true;
}

template<typename T>
void BPTree<T>::cascadeInsert(BPTree::TreeNode node) {//需要分裂的插入情况
    T key;
    TreeNode sibling = node->split(key);//分裂
    nodeCount++;
    if (node->isRoot()) {//要分裂的node是root，加一层
        TreeNode root = new BPTreeNode<T>(degree, false);
        level++;
        nodeCount++;
        this->root = root;
        node->parent = root;
        sibling->parent = root;
        root->add(key);
        root->children[0] = node;
        root->children[1] = sibling;
    }
    else {//
        TreeNode parent = node->parent;
        int index = parent->add(key);
        parent->children[index + 1] = sibling;
        sibling->parent = parent;
        if (parent->cnt == degree) {//需要继续分裂
            cascadeInsert(parent);
        }
    }
}

template<typename T>
bool BPTree<T>::remove(const T &key) {
    Nodemap<T> res;
    if (!root) {
        cerr << "Dequeuing empty BPTree!" << endl;
        return false;
    }
    if (!findKeyFromNode(root, key, res)) {
        cerr << "Key not found!" << endl;
        return false;
    }
    if (res.node->isRoot()) {//要删的结点是根的话
        res.node->removeAt(res.index);//删除根结点中的一个index
        keyCount--;
        return cascadeDelete(res.node);
    }
    else {//要删的结点不是根
        if (res.index == 0 && head != res.node) {
            // cascadingly update parent node
            int index;
            TreeNode currentParent = res.node->parent;
            bool keyFound = currentParent->search(key, index);
            while (!keyFound) {//找到第一个有被删掉的那个node的key的非叶子的父亲
                if (!currentParent->parent) { break; }
                currentParent = currentParent->parent;
                keyFound = currentParent->search(key, index);
            }
            currentParent->keys[index] = res.node->keys[1];
            res.node->removeAt(res.index);
            keyCount--;
            return cascadeDelete(res.node);
        }
        else{
            res.node->removeAt(res.index);
            keyCount--;
            return cascadeDelete(res.node);
        }
    }
}

template<typename T>
bool BPTree<T>::cascadeDelete(BPTree::TreeNode node) {
    int minimal = degree / 2, minimalBranch = (degree - 1) / 2;
    if ((node->isLeaf && node->cnt >= minimal) // leaf node
        || (node->isRoot() && node->cnt) // root node
        || (!node->isLeaf && !node->isRoot() && node->cnt >= minimal) // branch node
            ) {
        return true; // no need to update
    }
    if (node->isRoot()) {
        if (root->isLeaf) {//根是叶子，整棵树就一个node
            // tree completely removed
            root = nullptr;
            head = nullptr;
        }
        else {
            // reduce level by one
            root = node->children[0];
            root->parent = nullptr;
        }
        delete node;
        nodeCount--;
        level--;
        return true;
    }
    TreeNode currentParent = node->parent, sibling;
    int index;
    if (node->isLeaf) {
        // merge if it is leaf node
        currentParent->search(node->keys[0], index);
        if (currentParent->children[0] != node && currentParent->cnt == index + 1) {
            // rightest, also not first, merge with left sibling
            sibling = currentParent->children[index];
            if (sibling->cnt > minimal) {
                // transfer rightest of left to the leftest to meet the requirement
                return deleteLeafLL(node, currentParent, sibling, index);
            } else {
                // have to merge and cascadingly merge
                return deleteLeafLR(node, currentParent, sibling, index);
            }
        }
        else {
            // can merge with right brother
            if (currentParent->children[0] == node) {
                // on the leftest
                sibling = currentParent->children[1];
            }
            else {
                // normally
                sibling = currentParent->children[index + 2];
            }
            if (sibling->cnt > minimal) {
                // add the leftest of sibling to the right
                return deleteLeafRL(node, currentParent, sibling, index);
            }
            else {
                // merge and cascadingly delete
                return deleteLeafRR(node, currentParent, sibling, index);
            }
        }
    }
    else {
        // merge if it is branch node
        currentParent->search(node->children[0]->keys[0], index);
        if (currentParent->children[0] != node && currentParent->cnt == index + 1) {
            // can only be updated with left sibling
            sibling = currentParent->children[index];
            if (sibling->cnt > minimalBranch) {
                // add rightest key to the first node to avoid cascade operation
                return deleteBranchLL(node, currentParent, sibling, index);
            } else {
                // delete this and merge
                return deleteBranchLR(node, currentParent, sibling, index);
            }
        } else {
            // update with right sibling
            if (currentParent->children[0] == node) {
                sibling = currentParent->children[1];
            } else {
                sibling = currentParent->children[index + 2];
            }
            if (sibling->cnt > minimalBranch) {
                // add first key of sibling to the right
                return deleteBranchRL(node, currentParent, sibling, index);
            } else {
                // merge the sibling to current node
                return deleteBranchRR(node, currentParent, sibling, index);
            }
        }
    }
}

template<typename T>
bool BPTree<T>::deleteLeafLL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // transfer rightest of left to the leftest to meet the requirement
    for (int i = node->cnt; i > 0; i--) {
        node->keys[i] = node->keys[i - 1];
        node->keyOffset[i] = node->keyOffset[i - 1];
    }
    node->keys[0] = sibling->keys[sibling->cnt - 1];
    node->keyOffset[0] = sibling->keyOffset[sibling->cnt - 1];
    sibling->removeAt(sibling->cnt - 1);
    node->cnt++;
    parent->keys[index] = node->keys[0];
    return true;
}

template<typename T>
bool BPTree<T>::deleteLeafLR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    //merge and cascadingly merge
    parent->removeAt(index);
    for (int i = 0; i < node->cnt; i++) {
        sibling->keys[i + sibling->cnt] = node->keys[i];
        sibling->keyOffset[i + sibling->cnt] = node->keyOffset[i];
    }
    sibling->cnt += node->cnt;
    sibling->sibling = node->sibling;
    delete node;
    nodeCount--;
    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteLeafRL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // add the leftest of sibling to the right
    node->keys[node->cnt] = sibling->keys[0];
    node->keyOffset[node->cnt] = sibling->keyOffset[0];
    node->cnt++;
    sibling->removeAt(0);
    if (parent->children[0] == node) {
        parent->keys[0] = sibling->keys[0]; // if it is leftest, change key at index zero
    } else {
        parent->keys[index + 1] = sibling->keys[0]; // or next sibling should be updated
    }
    return true;
}

template<typename T>
bool BPTree<T>::deleteLeafRR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    //merge and cascadingly delete
    for (int i = 0; i < sibling->cnt; i++) {
        node->keys[node->cnt + i] = sibling->keys[i];
        node->keyOffset[node->cnt + i] = sibling->keyOffset[i];
    }
    if (node == parent->children[0]) {
        parent->removeAt(0); // if leftest, merge with first sibling
    } else {
        parent->removeAt(index + 1); // or merge with next
    }
    node->cnt += sibling->cnt;
    node->sibling = sibling->sibling;
    delete sibling;
    nodeCount--;
    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteBranchLL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // add rightest key to the first node to avoid cascade operation
    node->children[node->cnt + 1] = node->children[node->cnt];
    for (int i = node->cnt; i > 0; i--) {
        node->children[i] = node->children[i - 1];
        node->keys[i] = node->keys[i - 1];
    }
    node->children[0] = sibling->children[sibling->cnt];
    node->keys[0] = parent->keys[index];
    parent->keys[index] = sibling->keys[sibling->cnt - 1];
    node->cnt++;
    sibling->children[sibling->cnt]->parent = node;
    sibling->removeAt(sibling->cnt - 1);
    return true;
}

template<typename T>
bool BPTree<T>::deleteBranchLR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // delete this and merge
    sibling->keys[sibling->cnt] = parent->keys[index]; // add one node
    parent->removeAt(index);
    sibling->cnt++;
    for (int i = 0; i < node->cnt; i++) {
        node->children[i]->parent = sibling;
        sibling->children[sibling->cnt + i] = node->children[i];
        sibling->keys[sibling->cnt + i] = node->keys[i];
    }
    // rightest children
    sibling->children[sibling->cnt + node->cnt] = node->children[node->cnt];
    sibling->children[sibling->cnt + node->cnt]->parent = sibling;
    sibling->cnt += node->cnt;
    delete node;
    nodeCount--;
    return cascadeDelete(parent);
}

template<typename T>
bool BPTree<T>::deleteBranchRL(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // add first key of sibling to the right
    sibling->children[0]->parent = node;
    node->children[node->cnt + 1] = sibling->children[0];
    node->keys[node->cnt] = sibling->children[0]->keys[0];
    node->cnt++;

    if (node == parent->children[0]) {
        parent->keys[0] = sibling->keys[0];
    } else {
        parent->keys[index + 1] = sibling->keys[0];
    }

    sibling->children[0] = sibling->children[1];
    sibling->removeAt(0);
    return true;
}

template<typename T>
bool BPTree<T>::deleteBranchRR(BPTree::TreeNode node, BPTree::TreeNode parent, BPTree::TreeNode sibling, int index) {
    // merge the sibling to current node
    node->keys[node->cnt] = parent->keys[index];
    if (node == parent->children[0]) {
        parent->removeAt(0);
    } else {
        parent->removeAt(index + 1);
    }
    node->cnt++;
    for (int i = 0; i < sibling->cnt; i++) {
        sibling->children[i]->parent = node;
        node->children[node->cnt + i] = sibling->children[i];
        node->keys[node->cnt + i] = sibling->keys[i];
    }
    // rightest child
    sibling->children[sibling->cnt]->parent = node;
    node->children[node->cnt + sibling->cnt] = sibling->children[sibling->cnt];
    node->cnt += sibling->cnt;
    delete sibling;
    nodeCount--;
    return cascadeDelete(parent);
}

