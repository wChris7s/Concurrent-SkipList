#include <iostream>
#include <limits>
#include <ctime>
#include <random>
#include <cmath>
#include <mutex>
#include <memory>
#include <vector>
#include <random>
#include <thread>
#include <map>
#include "SkipList.h"

using namespace std;


struct random_data {
public:
    random_data(int min, int max) : min(min), max(max) {};
    int get_random_int() const;

private:
    int min;
    int max;
};

int random_data::get_random_int() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(gen);
}

template <class T>
struct add {
    int min, max;
    //SkipList *ptr;
    SkipList<T> *ptr_;

    //add(int min, int max, SkipList *ptr) : min(min), max(max), ptr(ptr) {};
    add(int min, int max, SkipList<T> *ptr_) : min(min), max(max), ptr_(ptr_) {};

/*    void operator()(int op) {
        random_data rd(min, max);
        for (int i = 0; i < op; i++) {
            ptr->add(rd.get_random_int());
        }
    }*/

    void operator()(int op) {
        random_data rd(min, max);
        for (int i = 0; i < op; i++) {
            ptr_->insert(rd.get_random_int());
        }
    }
};


template <class T>
struct rem {
    int min, max;
    //SkipList *ptr;
    SkipList<T> *ptr_;

    //add(int min, int max, SkipList *ptr) : min(min), max(max), ptr(ptr) {};
    rem(int min, int max, SkipList<T> *ptr_) : min(min), max(max), ptr_(ptr_) {};

/*    void operator()(int op) {
        random_data rd(min, max);
        for (int i = 0; i < op; i++) {
            ptr->add(rd.get_random_int());
        }
    }*/

    void operator()(int op) {
        random_data rd(min, max);
        for (int i = 0; i < op; i++) {
            ptr_->remove(rd.get_random_int());
        }
    }
};


int main() {
    SkipList<int> Sk(10);

    std::vector<std::thread> insert_t;
    std::vector<std::thread> rem_t;
    unsigned int t_threads = std::thread::hardware_concurrency();

    for (unsigned int i = 0; i < t_threads; i++) {
        insert_t.emplace_back(std::thread(add(0, 100, &Sk), 100));
    }
    for (unsigned int i = 0; i < t_threads; i++) {
        insert_t[i].join();
    }
    Sk.print();

    for (unsigned int i = 0; i < t_threads; i++) {
        rem_t.emplace_back(std::thread(rem(0, 50, &Sk), 50));
    }
    for (unsigned int i = 0; i < t_threads; i++) {
        rem_t[i].join();
    }

    Sk.print();

    return 0;
}
