//
// Created by chris on 4/12/2022.
//

#ifndef CONCURRENT_SKIPLIST_SKIP_LIST_NODE_H
#define CONCURRENT_SKIPLIST_SKIP_LIST_NODE_H

#include <vector>

template<class ElemType>
struct node {
    std::vector<node *> forward;
    ElemType key; //  key
    node(int lvl, const ElemType &key);
};

template<class ElemType>
node<ElemType>::node(int lvl, const ElemType &key) : key(key), forward(lvl, nullptr) {};

#endif //CONCURRENT_SKIPLIST_SKIP_LIST_NODE_H
