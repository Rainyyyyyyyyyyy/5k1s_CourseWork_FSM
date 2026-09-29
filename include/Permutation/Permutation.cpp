#include "Permutation.h"

// Построить перестановку pi из записи цикла:
// cycle = [0, c1, c2, ..., c_{N-1}]
// pi[cycle[i]] = cycle[(i+1) % N]
std::vector<int> cycle_to_perm(const std::vector<int> &cycle)
{
    size_t n = cycle.size();
    std::vector<int> pi(n);
    for (size_t i = 0; i < n; ++i)
    {
        pi[cycle[i]] = cycle[(i + 1) % n];
    }
    return pi;
}

// Проверка: ровно один цикл (на всякий случай, для теста)
bool is_single_cycle(const std::vector<int> &pi)
{
    size_t n = pi.size();
    std::vector<bool> visited(n, false);
    int cycles = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            ++cycles;
            if (cycles > 1)
                return false;
            size_t x = i;
            while (!visited[x])
            {
                visited[x] = true;
                x = pi[x];
            }
        }
    }
    return true;
}

// Печать перестановки в виде pi и в виде цикла
void print_perm(const std::vector<int> &pi, const std::vector<int> &cycle)
{
    std::cout << "pi = [";
    for (size_t i = 0; i < pi.size(); ++i)
    {
        std::cout << pi[i] << (i + 1 < pi.size() ? ", " : "");
    }
    std::cout << "]  cycle = ";
    for (size_t i = 0; i < cycle.size(); ++i)
    {
        std::cout << cycle[i] << " -> ";
    }
    std::cout << cycle[0] << "\n";
}

Permut Permut::operator*(const Permut &other) const
{
    std::vector<size_t> result(perm.size());
    for (size_t i = 0; i < perm.size(); ++i)
    {
        result[i] = perm[other.perm[i]];
    }
    return Permut(std::move(result));
}

Permut &Permut::operator*=(const Permut &other)
{
    *this = *this * other;
    return *this;
}

Permut Permut::operator^(size_t exp) const
{
    //Permut result(std::vector<size_t>(perm.size()));
    // std::iota(result.perm.begin(), result.perm.end(), 0);

    Permut result(perm.size());
    Permut base = *this;
    while (exp > 0)
    {
        if (exp & 1)
        {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

Permut &Permut::operator^=(size_t exp)
{
    *this = *this ^ exp;
    return *this;
}

Permut Permut::GetInverse() const
{
    std::vector<size_t> result(perm.size());
    for (size_t i = 0; i < perm.size(); ++i)
    {
        result[perm[i]] = i;
    }
    return Permut(std::move(result));
}

// сброс перестановки до тождественной
void Permut::Reset()
{
    size_t n = perm.size();
    for (size_t i = 0; i < n; i++)
        perm[i] = i;
}

void Permut::Print() const
{
    for (size_t i = 0; i < perm.size(); ++i)
    {
        std::cout << i << '\t';
    }
    std::cout << '\n';
    for (size_t i = 0; i < perm.size(); ++i)
    {
        std::cout << perm[i] << '\t';
    }
    std::cout << '\n';
}