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
    std::vector<size_t> result(perm.size());

    for (size_t i = 0; i < perm.size(); ++i)
    {
        result[i] = perm[other.perm[i]];
    }

    perm.swap(result);
    return *this;
}

Permut Permut::operator^(size_t exp) const
{
    Permut result(perm.size(), 0);
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

void Permut::Print(std::ostream &os) const
{
    for (size_t i = 0; i < perm.size(); ++i)
    {
        os << i << '\t';
    }
    os << '\n';
    for (size_t i = 0; i < perm.size(); ++i)
    {
        os << perm[i] << '\t';
    }
    os << '\n';
}

// Получить разложение перестановки на её циклы.
// Возвращает вектор циклов, каждый цикл представлен в виде вектора.
std::vector<std::vector<size_t>> Permut::GetCycles() const
{

    std::vector<std::vector<size_t>> cycles;
    std::vector<bool> visited(perm.size(), false);

    for (size_t i = 0; i < perm.size(); ++i)
    {
        if (!visited[i])
        {
            std::vector<size_t> cycle;
            size_t current = i;

            do
            {
                cycle.push_back(current);
                visited[current] = true;
                current = perm[current];
            } while (current != i);

            cycles.push_back(cycle);
        }
    }

    return cycles;
}

// печать перестановки в виде цикла
void Permut::PrintInCycleView(std::ostream &os) const
{

    std::vector<std::vector<size_t>> cycles = GetCycles();
    for (const auto &cycle : cycles)
    {
        os << '(';
        for (size_t i = 0; i < cycle.size(); ++i)
        {
            os << cycle[i] << ' ';
        }
        os << cycle[0];
        os << ") ";
    }
}
// печать перестановки в виде цикла с разделителем табуляции ('\t')
void Permut::PrintInCycleViewWithTab(std::ostream &os) const
{
    std::vector<std::vector<size_t>> cycles = GetCycles();
    for (const auto &cycle : cycles)
    {
        os << '(';
        for (size_t i = 0; i < cycle.size(); ++i)
        {
            os << cycle[i] << ' ';
        }
        os << cycle[0];
        os << ")\t";
    }
}

bool Permut::Validate() const

{
    std::vector<bool> seen(perm.size(), false);
    for (size_t i = 0; i < perm.size(); ++i)
    {
        if (perm[i] >= perm.size() || seen[perm[i]])
            return false;
        seen[perm[i]] = true;
    }
    return true;
}

bool Permut::Validate(const std::vector<size_t> &pi) const
{
    std::vector<bool> seen(pi.size(), false);
    for (size_t i = 0; i < pi.size(); ++i)
    {
        if (pi[i] >= pi.size() || seen[pi[i]])
            return false;
        seen[pi[i]] = true;
    }
    return true;
}
