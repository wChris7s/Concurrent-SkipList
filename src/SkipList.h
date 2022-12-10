//
// Created by chris on 9/12/2022.
//

#ifndef CONCURRENT_SKIPLIST_SKIPLIST_H
#define CONCURRENT_SKIPLIST_SKIPLIST_H

#include <vector>
#include <climits>
#include <random>
#include <map>
#include <iostream>
#include "node.h"

template <class T>
class SkipList {
public:
    explicit SkipList(int max_lvl);
    bool remove(T key);
    bool insert(T key);
    bool find(T key);
    void print();
private:
    node<T> *head;
    node<T> *tail;
    unsigned int max_lvl;
    int find(T searchKey, std::vector<node<T>*> *preds, std::vector<node<T>*> *succs);
    bool okToDelete(node<T>* candidate, int found);
    void unlock(std::vector<node<T>*> *preds, int highestLocked);
    bool get_random_bool();
    unsigned int randomLevel();
};

template class SkipList<int>;
template class SkipList<float>;
template class SkipList<char>;
template class SkipList<double>;

#endif //CONCURRENT_SKIPLIST_SKIPLIST_H
