//
// Created by chris on 4/12/2022.
//

#ifndef CONCURRENT_SKIPLIST_SKIP_LIST_H
#define CONCURRENT_SKIPLIST_SKIP_LIST_H

#include "skip-list-node.h"
#include <random>

template<class ElemType>
class SkipList {
public:
    explicit SkipList(int max_lvl);
    ~SkipList();
    bool get_random_bool();
    bool insert(ElemType key);
    bool erase(ElemType key);
    node<ElemType> *find(std::vector<node<ElemType> *> *prev, ElemType key);
    void print();

    node<ElemType> header;  // header(max_lvl, key={}})
    int current_lvl;        // 1
    int max_lvl;            // n
};

template<class ElemType>
SkipList<ElemType>::SkipList(int max_lvl)
        : max_lvl(max_lvl), header(max_lvl, {}), current_lvl(1) {}

template<class ElemType>
bool SkipList<ElemType>::get_random_bool() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, 1);
    return distribution(gen);
}

template<class ElemType>
void SkipList<ElemType>::print() {
    typedef node<ElemType> *nd_p;
    using std::cout, std::endl;
    nd_p p = &header;
    for (int i = max_lvl - 1; i >= 0; i--) {
        cout << "[" << i << "] -> ";
        if (p->forward[i] == nullptr) {
            cout << "NIL" << endl;
        } else {
            nd_p q = p->forward[i];
            while (q != nullptr) {
                cout << q->key << " ";
                q = q->forward[i];
            }
            cout << endl;
        }
    }
    cout << "-H-" << endl;
}

template<class ElemType>
node<ElemType> *SkipList<ElemType>::find(std::vector<node<ElemType> *> *prev, ElemType key) {
    prev->resize(max_lvl, nullptr);
    node<ElemType> *next = &header;
    for (int i = current_lvl - 1; 0 <= i; --i) {
        while (next->forward[i] != nullptr && next->forward[i]->key < key) {
            next = next->forward[i];
        }
        (*prev)[i] = next;
        // [prev] se queda con el valor anterior al que se busca
        // asi mismo almacena los nodos recorridos anteriormente.
    }
    next = next->forward[0];    // obtiene el siguiente número del cual se busca
    // [prev] -> [-] -> [next]
    return next;
}

template<class ElemType>
bool SkipList<ElemType>::insert(ElemType key) {
    std::vector<node<ElemType> *> prev; // vector con los nodos previos al que se quiere insertar
    node<ElemType> *next = find(&prev, key);    // nodo siguiente al nodo que se quiere insertar

    if (next != nullptr && next->key == key) {
        // si nodo next es diferente de nullptr y la key siguiente es igual de la key a insertar
        // no se inserta nada, retorna false.
        return false;
    }

    unsigned int level = 1;
    while (get_random_bool() == true && level < max_lvl) {
        // si la "moneda" cae cara, el nivel va aumentando, caso contrario no aumenta.
        level++;
    }   // Este nivel determinara en que nivel de las LE. se insertara el dato.

    if (current_lvl < level) {
        // Si el nuevo nivel que tendrá el nodo a insertar es mayor que el nivel actual de la SkipList
        // se hace que el nodo prev o antecesor apunte al head.
        for (int i = current_lvl; i < level; i++) {
            prev[i] = &header;
        }
        current_lvl = level;    // Se actualiza el nuevo nivel
    }

    auto *new_node = new node<ElemType>(level, key);    // Generamos un nuevo nodo torré con su respectivo key.
    for (unsigned int i = 0; i < level; ++i) {
        new_node->forward[i] = prev[i]->forward[i];
        prev[i]->forward[i] = new_node;
    }   // Modificación de los punteros.
    return true;
}

template<class ElemType>
bool SkipList<ElemType>::erase(ElemType key) {
    std::vector<node<ElemType> *> prev;
    node<ElemType> *next = find(&prev, key);
    if (next != nullptr && next->key == key) {
        for (int i = 0; i <= current_lvl - 1; i++) {
            if (prev[i]->forward[i] != next)
                break;
            prev[i]->forward[i] = next->forward[i];
        }
        while (current_lvl - 1 > 0 && header.forward[current_lvl - 1] == nullptr) {
            current_lvl--;  // Se decremente hasta que llegue a un nivel adecuado
        }   // Si el nivel actual es diferente de nulo entonces se encuentra en un nivel adecuado.
        return true;
    } else {
        return false;
    }
}

template<class ElemType>
SkipList<ElemType>::~SkipList() {
    auto *node = &header;
    while (node->forward[0]) {
        auto *tmp = node->forward[0];
        delete node;
        node = tmp;
    }
}

#endif //CONCURRENT_SKIPLIST_SKIP_LIST_H
