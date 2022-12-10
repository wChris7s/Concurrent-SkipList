//
// Created by chris on 9/12/2022.
//

#ifndef CONCURRENT_SKIPLIST_GENERATE_DATA_H
#define CONCURRENT_SKIPLIST_GENERATE_DATA_H

#include <vector>
#include <random>

template <class T>
struct randomData {
    std::vector<T> data;    // vector contenedor
    int min, max;   // minimo elemento posible, maximo elemento posible
    unsigned int t_elements;    // Cantidad total de elementos a generar/insertar/borrar/buscar
    explicit randomData(unsigned int t_elements, int min, int max);
    randomData(int min, int max);

    void stratified_random_sample_integers();
    void stratified_random_sample_reals();
    T get_random_integer ();
    T get_random_real ();
};

template<class T>
randomData<T>::randomData(int min, int max) : data(nullptr), t_elements(0) {};

template <class T>
randomData<T>::randomData(unsigned int t_elements, int min, int max) : t_elements(t_elements),
    data(t_elements), min(min), max(max) {};

template<class T>
T randomData<T>::get_random_integer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(gen);
}

template<class T>
T randomData<T>::get_random_real() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> distribution(min, max);
    return distribution(gen);
}


template <class T>
void randomData<T>::stratified_random_sample_integers() {
    if (t_elements >= 3) {
        int total = t_elements/3;
        for(int i = 0; i < total ; i++) {
            data[i] = get_random_integer();
        }
        total += total;
        for(int i = 0; i < total ; i++) {
            data[i] = get_random_integer();
        }
        for(int i = 0; i < t_elements; i++) {
            data[i] = get_random_integer();
        }
        return;
    } else {
        return;
    }
};

template <class T>
void randomData<T>::stratified_random_sample_reals() {
    if (t_elements >= 3) {
        int total = t_elements/3;
        for(int i = 0; i < total ; i++) {
            data[i] = get_random_real();
        }
        total += total;
        for(int i = 0; i < total ; i++) {
            data[i] = get_random_real();
        }
        for(int i = 0; i < t_elements; i++) {
            data[i] = get_random_real();
        }
        return;
    } else {
        return;
    }
};

#endif //CONCURRENT_SKIPLIST_GENERATE_DATA_H
