/*
Сбор статистики по расстояниям единственности для различных (n,m)-генераторов
*/

#include "Permutation.h"
#include "FSM.h"
#include "Reverse.h"

#include <chrono>
#include <random>

int main(){
    std::mt19937 random_generator(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    //srand(time(NULL));
    Permut g20(8);
    g20.Print();
    return 1;
}
