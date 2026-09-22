#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <algorithm>
#include <iostream>
#include <vector>

class Permut
{
private:
    std::vector<size_t> perm;

public:
    // конструктор создаёт тождественную перестановку размера n
    explicit Permut(const size_t &n)
    {
        perm.reserve(n);
        for (size_t i = 0; i < n; ++i)
            perm.push_back(i);
    }
    Permut(const std::vector<size_t> &p) : perm(p) {}
    Permut(std::vector<size_t> &&p) : perm(std::move(p)) {}
    Permut(const Permut &d) { perm = d.perm; }
    Permut(Permut &&d) { perm = std::move(d.perm); }
    ~Permut() = default;
    Permut &operator=(const Permut &) = default;
    Permut &operator=(Permut &&) = default;

    Permut operator*(const Permut &other) const;

    Permut &operator*=(const Permut &other);

    Permut operator^(size_t exp) const;

    Permut &operator^=(size_t exp);

    Permut GetInverse() const;

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