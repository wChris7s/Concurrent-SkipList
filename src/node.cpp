//
// Created by chris on 9/12/2022.
//

#include "node.h"


template<class T>
node<T>::node(unsigned int lvl, T key): key(key), top_level(lvl), marked(false),
                                        f_linked(false), forward(lvl + 1, nullptr) {};
template<class T>
void node<T>::lock() { n_lock.lock(); }
template<class T>
void node<T>::unlock() { n_lock.unlock(); }
