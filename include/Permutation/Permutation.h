#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>


namespace {
    template <typename T>
    void swap(T &a, T &b)
    {
        T c = a;
        a = b;
        b = c;
    }
} // namespace

class Permut
{
private:
    std::vector<size_t> perm;

    static std::mt19937 &defaultGenerator()
    {
        static std::mt19937 generator(std::random_device{}());
        return generator;
    }

public:
    // конструктор создаёт тождественную перестановку размера n
    // explicit Permut(const size_t &n)
    // {
    //     perm.reserve(n);
    //     for (size_t i = 0; i < n; ++i)
    //         perm.push_back(i);
    // }
    // конструктор по vector<size_t>
    Permut(const std::vector<size_t> &p) : perm(p) {}

    // конструктор по перемещению vector<size_t>
    Permut(std::vector<size_t> &&p) : perm(std::move(p)) {}

    // конструктор по копированию
    Permut(const Permut &d) { perm = d.perm; }

    // конструктор по перемещению
    Permut(Permut &&d) { perm = std::move(d.perm); }

    // конструктор по умолчанию
    ~Permut() = default;

    // конструктор по умолчанию с параметрами размера и полноцикловости
    // t = 0: генерирует тождественную перестановку
    // t = 1: генерирует тривиальную полноцикловую: p(x) = x+1 mod N;
    // t = 2: генерирует случайную полноцикловую перестановку
    // t = 3: генерирует случайную перестановку
    explicit Permut(const size_t &N, char t = 2)
        : Permut(N, t, defaultGenerator())
    {
    }

    Permut(const size_t &N, char t, std::mt19937 &generator){
        if(t == 2){
            perm.resize(N);
            if(N == 0)
                return;

            // Вставляем каждую новую вершину в случайное место текущего цикла.
            perm[0] = 0;
            for(size_t i = 1; i < N; ++i){
                std::uniform_int_distribution<size_t> distribution(0, i - 1);
                size_t predecessor = distribution(generator);
                perm[i] = perm[predecessor];
                perm[predecessor] = i;
            }
            return;
        } else
        if(t == 0){
            perm.resize(N);
            for(size_t i=0; i<N; i++)perm[i] = i;
            return;
        }
        if(t == 1){
            perm.resize(N);
            if(N == 0)
                return;
            for(size_t i=0; i<N-1; i++)perm[i] = i+1;
            perm[N-1] = 0;
            return;
        }
        if(t == 3){
            perm.resize(N);
            for(size_t i=0; i<N; i++)perm[i] = i;
            if(N == 0)
                return;
            for(size_t i=0; i<N; i++){
                size_t j = rand()%N;
                swap(perm[i], perm[j]);
            }
        }
    }
    Permut &operator=(const Permut &) = default;
    Permut &operator=(Permut &&) = default;

    Permut operator*(const Permut &other) const;

    Permut &operator*=(const Permut &other);

    Permut operator^(size_t exp) const;

    Permut &operator^=(size_t exp);

    Permut GetInverse() const;

    std::vector<size_t> Get() const { return perm; }

    // сброс перестановки до тождественной
    void Reset();

    void Print() const;
};

// Построить перестановку pi из записи цикла:
// cycle = [0, c1, c2, ..., c_{N-1}]
// pi[cycle[i]] = cycle[(i+1) % N]
std::vector<int> cycle_to_perm(const std::vector<int> &cycle);

// Проверка: ровно один цикл (на всякий случай, для теста)
bool is_single_cycle(const std::vector<int> &pi);

// Печать перестановки в виде pi и в виде цикла
void print_perm(const std::vector<int> &pi, const std::vector<int> &cycle);

#endif // PERMUTATION_H