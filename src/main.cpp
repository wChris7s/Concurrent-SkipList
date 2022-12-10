#include <vector>
#include <thread>
#include <ctime>
#include "SkipList.h"
#include "generate_data.h"

using namespace std;

#define MAX_LEVEL 12

template<class T>
struct add {
    SkipList<T> *tmp_sk;
    randomData<T> *rd;

    add(SkipList<T> *tmp_sk, randomData<T> *rd)
            : tmp_sk(tmp_sk), rd(rd) {};

    void operator()() {
        for (int i = 0; i < rd->t_elements; i++) {
            tmp_sk->insert(rd->data[i]);
        }
    }
};

template<class T>
struct rem {
    SkipList<T> *tmp_sk;
    randomData<T> *rd;

    rem(SkipList<T> *tmp_sk, randomData<T> *rd)
            : tmp_sk(tmp_sk), rd(rd) {};

    void operator()() {
        for (int i = 0; i < rd->t_elements; i++) {
            tmp_sk->remove(rd->data[i]);
        }
    }
};

template<class T>
struct _find {
    SkipList<T> *tmp_sk;
    randomData<T> *rd;

    _find(SkipList<T> *tmp_sk, randomData<T> *rd)
            : tmp_sk(tmp_sk), rd(rd) {};

    void operator()() {
        for (int i = 0; i < rd->t_elements; i++) {
            //std::cout << tmp_sk->find(rd->data[i]) << "-";
            tmp_sk->find(rd->data[i]);
        }
        //std::cout << std::endl;
    }
};

int main() {
    // max := número aleatorio a generar.
    // min := número aleatorio a generar.
    // t_elements := cantidad de elementos a insertar/borrar/buscar.
    std::cout.precision(8);
    int min = 0, t_elements = 50000, max = t_elements * 3;
    SkipList<int> Sk(MAX_LEVEL);
    randomData<int> rd(t_elements, min, max);
    std::cout << "---------- Creando Datos ----------" << std::endl;
    rd.stratified_random_sample_integers();
    std::cout << "---------- Datos Creados ----------" << std::endl << std::endl;

    double time, t1, t2;
    std::vector<std::thread> insert_t;
    std::vector<std::thread> rem_t;
    std::vector<std::thread> search_t;
    //  unsigned int t_threads = std::thread::hardware_concurrency();
    unsigned int t_threads = 8;


    std::cout << "- Insertando datos en la SkipList -" << std::endl;
    t1 = clock();
    for (unsigned int i = 0; i < t_threads; i++) {
        insert_t.emplace_back(add<int>(&Sk, &rd));
    }
    for (unsigned int i = 0; i < t_threads; i++) {
        insert_t[i].join();
    }
    t2 = clock();

    time = (t2-t1)/CLOCKS_PER_SEC;
    std::cout << ":: " << time << std::endl << std::endl;


    std::cout << "-- Buscando datos en la SkipList --" << std::endl;
    t1 = clock();
    for (unsigned int i = 0; i < t_threads; i++) {
        search_t.emplace_back(_find<int>(&Sk, &rd));
    }
    for (unsigned int i = 0; i < t_threads; i++) {
        search_t[i].join();
    }
    t2 = clock();

    time = (t2-t1)/CLOCKS_PER_SEC;
    std::cout << ":: " << time << std::endl << std::endl;


    std::cout << "-- Borrando datos en la SkipList --" << std::endl;
    t1 = clock();
    for (unsigned int i = 0; i < t_threads; i++) {
        rem_t.emplace_back(rem<int>(&Sk, &rd));
    }
    for (unsigned int i = 0; i < t_threads; i++) {
        rem_t[i].join();
    }
    t2 = clock();


    time = (t2-t1)/CLOCKS_PER_SEC;
    std::cout << ":: " << time << std::endl << std::endl;
    return 0;
}
