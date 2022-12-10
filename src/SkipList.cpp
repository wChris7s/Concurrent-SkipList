//
// Created by chris on 9/12/2022.
//

#include "SkipList.h"

template <class T>
SkipList<T>::SkipList(int max_lvl) : max_lvl(max_lvl){
    head = new node<T>(max_lvl, std::numeric_limits<T>::min());
    tail = new node<T>(max_lvl, std::numeric_limits<T>::max());
    for(int i = 0; i < head->forward.size(); i++)
        head->forward[i] = tail;
}

template<class T>
bool SkipList<T>::okToDelete(node<T> *candidate, int found) {
    return (candidate->f_linked && candidate->top_level == found && !candidate->marked);
}

template <class T>
int SkipList<T>::find(T searchKey, std::vector<node<T>*> *preds, std::vector<node<T>*> *succs) {
    int found = -1;
    node<T> *pred = head;
    for (int i = max_lvl; i >= 0; i--) {
        node<T> *curr = pred->forward[i];
        while (searchKey > curr->key) {
            pred = curr;
            curr = pred->forward[i];
        }
        if (found == -1 && searchKey == curr->key) {
            found = i;
        }
        (*preds)[i] = pred;
        (*succs)[i] = curr;
    }
    return found;
}

template <class T>
bool SkipList<T>::insert(T key) {
    unsigned int topLevel = randomLevel();
    std::vector<node<T>*> preds(max_lvl + 1, nullptr), succs(max_lvl + 1, nullptr);
    while (true) {
        int found = find(key, &preds, &succs);
        if (found != -1) {
            node<T>* nodeFound = succs[found];
            if (!nodeFound->marked) {
                while (!nodeFound->f_linked);
                return false;
            }
            continue;
        }
        int highestLocked = -1;
        node<T> *pred, *succ, *prevPred = nullptr;
        bool valid = true;
        for (int level = 0; valid && level <= topLevel; level++){
            pred = preds[level];
            succ = succs[level];
            if (pred != prevPred) {
                pred->lock();
                highestLocked = level;
                prevPred = pred;
            }
            valid = !pred->marked && !succ->marked && pred->forward[level] == succ;
        }
        if (!valid) {
            for (int level = 0; level <= highestLocked; level++) {
                preds[level]->unlock();
            }
            continue;
        }
        node<T> *newNode = new node<T>(topLevel, key);
        for (int level = 0; level <= topLevel; level++){
            newNode->forward[level] = succs[level];
            preds[level]->forward[level] = newNode;
        }
        newNode->f_linked = true;
        for (int level = 0; level <= highestLocked; level++) {
            preds[level]->unlock();
        }
        return true;
    }
}

template <class T>
bool SkipList<T>::remove(T key) {
    node<T> *nodeToDelete = nullptr;
    bool isMarked = false;
    int topLevel = -1;
    std::vector<node<T>*> preds(max_lvl, nullptr), succs(max_lvl, nullptr);
    while (true){
        int found = find(key, &preds, &succs);
        if (isMarked || found != -1 && okToDelete(succs[found], found)){
            if (!isMarked) {
                nodeToDelete = succs[found];
                topLevel = nodeToDelete->top_level;
                nodeToDelete->lock();
                if (nodeToDelete->marked) {
                    nodeToDelete->unlock();
                    return false;
                }
                nodeToDelete->marked = true;
                isMarked = true;
                nodeToDelete->unlock();
            }
            int highestLocked = -1;
            node<T> *pred, *succ, *prevPred = nullptr;
            bool valid = true;
            for (int level = 0; valid && level <= topLevel; level++){
                pred = preds[level];
                succ = succs[level];
                if (pred != prevPred) {
                    pred->lock();
                    highestLocked = level;
                    prevPred = pred;
                }
                valid = !pred->marked && pred->forward[level] == succ;
            }
            if (!valid) {
                for (int level = 0; level <= highestLocked; level++) {
                    preds[level]->unlock();
                }
                continue;
            }
            for (int level = topLevel; level >= 0; level--){
                preds[level]->forward[level] = nodeToDelete->forward[level];
            }
            for (int level = 0; level <= highestLocked; level++) {
                preds[level]->unlock();
            }
            return true;
        } else return false;
    }
}

template <class T>
bool SkipList<T>::get_random_bool() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, 1);
    return distribution(gen);
}

template <class T>
unsigned int SkipList<T>::randomLevel() {
    unsigned int level = 0;
    while (get_random_bool() == true && level < max_lvl) {
        level++;
    }
    return level;
}

template <class T>
void SkipList<T>::print() {
    typedef node<T> *nd_p;
    using std::cout, std::endl;
    nd_p p = head;
    for (int i = max_lvl; i >= 0; i--) {
        cout << "[" << i << "]";
        if (p->forward[i] == nullptr) {
            cout << "NIL" << endl;
        } else {
            nd_p q = p->forward[i];
            cout << "-> ";
            while (q != nullptr && q != tail) {
                cout << q->key << " ";
                q = q->forward[i];
            }
            if (q->forward[i] == nullptr){
                cout << "-> NIL" << endl;
            }
        }
    }
    cout << "-H-" << endl;
}