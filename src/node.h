//
// Created by chris on 9/12/2022.
//

#ifndef CONCURRENT_SKIPLIST_NODE_H
#define CONCURRENT_SKIPLIST_NODE_H

#include <vector>
#include <mutex>
#include <atomic>


template<class T>
class node {
public:
    T key;
    unsigned int top_level;
    std::vector<node *> forward;
    std::atomic<bool> marked;
    std::atomic<bool> f_linked;
    std::mutex n_lock;

    node(unsigned int lvl, T key);
    void lock();
    void unlock();
};

template class node<int>;
template class node<float>;
template class node<char>;
template class node<double>;

#endif //CONCURRENT_SKIPLIST_NODE_H
