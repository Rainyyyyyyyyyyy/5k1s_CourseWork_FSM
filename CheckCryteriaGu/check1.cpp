/*
 * check1.cpp
 * Проверка утверждения
 * Пусть 𝑔2(1, •) − произведение двух независимых циклов 𝐶1,𝐶2 с длинами 𝐿1,𝐿2,
 *  𝐿1 + 𝐿2 = 2^m
 *
 * 𝑔2(0, •) − одна транспозиция между циклами 𝐶1,𝐶2 (или наоборот).
 *
 * 𝐺𝑢 = 𝑔2(𝑢[0]) ∗ 𝑔2(𝑢[1]) ∗ 𝑔2(𝑢[2]) ∗ …∗ 𝑔2(𝑢𝑢[2^𝑛−1])
 *  полноцикловая ∀𝑢 нечётного веса ⟺ 𝐿1 = 𝐿2
 */

#include "Permutation.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>



/*
 * pool = набор векторов веса right_steps
 * left_steps = кол-во нулей в векторе
 * right_steps = кол-во единиц в векторе
 */
namespace
{
    inline size_t C_n_k(const size_t &n, const size_t &k);

    // Быстрый генератор Госпера. Ограничен размером uint64_t.
    template <size_t N>
    std::vector<std::bitset<N>> generate_vectors_gosper(const size_t &k)
    {
        static_assert(N <= 63, "Алгоритм Госпера поддерживает N <= 63");

        std::vector<std::bitset<N>> result;
        if (k > N)
            return result;

        result.reserve(C_n_k(N, k));

        if (k == 0)
        {
            result.emplace_back(0);
            return result;
        }

        uint64_t sub = (static_cast<uint64_t>(1) << k) - 1;
        const uint64_t limit = static_cast<uint64_t>(1) << N;

        while (sub < limit)
        {
            result.emplace_back(sub);

            const uint64_t c = sub & (~sub + 1);
            if (c == 0)
                break;

            const uint64_t next = sub + c;
            if (next >= limit)
                break;

            sub = (((next ^ sub) >> 2) / c) | next;
        }

        return result;
    }

    // Универсальный генератор для произвольного размера N.
    template <size_t N>
    std::vector<std::bitset<N>> generate_vectors_next_permutation(const size_t &k)
    {
        std::vector<std::bitset<N>> result;
        // Если вес больше длины, таких векторов не существует
        if (k > N){ return result; }

        std::vector<bool> current(N, false);
        std::fill(current.end() - static_cast<std::ptrdiff_t>(k), current.end(), true);

        do
        {
            std::bitset<N> value;
            for (size_t i = 0; i < N; ++i)
            {
                value[N - 1 - i] = current[i];
            }
            result.push_back(value);
        }
        while (std::next_permutation(current.begin(), current.end()));

        return result;
    }

    struct BinaryTree
    {
        BinaryTree *left;
        BinaryTree *right;
        size_t data;

        // конструктор
        BinaryTree(BinaryTree *t_left = nullptr, BinaryTree *t_right = nullptr, const size_t &t_data = 0)
            : left(t_left), right(t_right), data(t_data) {}

        // деструктор
        ~BinaryTree()
        {
            delete left;
            delete right;
        }
    };

    inline size_t C_n_k(const size_t &n, const size_t &k)
    {
        if (k > n)
            return 0;

        const size_t reduced_k = std::min(k, n - k);
        size_t result = 1;

        for (size_t i = 1; i <= reduced_k; ++i)
        {
            size_t numerator = n - reduced_k + i;
            size_t denominator = i;

            const size_t first_gcd = std::gcd(numerator, denominator);
            numerator /= first_gcd;
            denominator /= first_gcd;

            const size_t second_gcd = std::gcd(result, denominator);
            result /= second_gcd;
            denominator /= second_gcd;

            if (denominator != 1 ||
                result > std::numeric_limits<size_t>::max() / numerator)
            {
                throw std::overflow_error("C(n, k) does not fit in size_t");
            }

            result *= numerator;
            result /= denominator;
        }

        return result;
    }

    void make_pool_fixed_weight(BinaryTree *cur_root,
                                std::vector<size_t> &pool,
                                size_t left_steps, size_t right_steps,
                                const size_t &WALL, size_t temp = 0, size_t deep = 0)
    {
        if (deep < WALL)
        {
            if (left_steps > 0)
            {
                BinaryTree *new_left = new BinaryTree(nullptr, nullptr, 0);
                new_left->data = temp = cur_root->data;
                cur_root->left = new_left;
                make_pool_fixed_weight(new_left, pool,
                                       left_steps - 1, right_steps,
                                       WALL, new_left->data, deep + 1);
            }
            if (right_steps > 0)
            {
                BinaryTree *new_right = new BinaryTree(nullptr, nullptr, 0);
                new_right->data = temp = cur_root->data | (static_cast<size_t>(1) << (WALL - deep - 1));
                cur_root->right = new_right;
                make_pool_fixed_weight(new_right, pool,
                                       left_steps, right_steps - 1,
                                       WALL, new_right->data, deep + 1);
            }
        }
        else
        {
            pool.push_back(temp);
        }
    }

    void make_pool(std::vector<size_t> &pool, size_t lenght, size_t max_degree)
    {
        for (int i = 1; i <= max_degree; i++)
        {
            BinaryTree tree;
            std::vector<size_t> pool_with_i_weight;
            make_pool_fixed_weight(&tree, pool_with_i_weight,
                                   lenght - i, i, lenght);

            pool.insert(pool.end(),
                        std::make_move_iterator(pool_with_i_weight.begin()),
                        std::make_move_iterator(pool_with_i_weight.end()));
        }
    }
} // namespace

void PrintInBinaryView(const size_t &s, const unsigned char &size)
{
    for (int i = size - 1; i >= 0; i--)
    {
        std::cout << ((s >> i) & 1);
    }
}

int main()
{
    constexpr size_t N = 6;
    constexpr size_t NP = static_cast<size_t>(1) << N;

    constexpr size_t M = 3;
    constexpr size_t MP = static_cast<size_t>(1) << M;

    //          0  1  2  3  4  5  6  7  8  9   10  11  12  13  14  15
    Permut g20({1, 2, 3, 0, 5, 6, 7, 4}); 
    Permut g21({4, 1, 2, 3, 0, 5, 6, 7});
    //Permut g20({1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 15, 8}); // 2 independent cycles
    //Permut g21({8, 1, 2, 3, 4, 5, 6, 7, 0, 9, 10, 11, 12, 13, 14, 15}); // one transposition between cycles


    g20.Print(std::cout);
    g20.PrintInCycleViewWithTab(std::cout);
    std::cout<<'\n';
    g21.Print(std::cout);
    g21.PrintInCycleViewWithTab(std::cout);
    std::cout<<'\n';
    Permut Gu(MP, 0);
    size_t input_w_u;
    std::cout << "Enter w_u: ";
    if (!(std::cin >> input_w_u) || input_w_u > NP)
    {
        std::cerr << "w_u must be in range [0, " << NP << "]\n";
        return 1;
    }
    const size_t w_u = input_w_u;

    std::vector<std::bitset<NP>> pool;
    if constexpr (NP <= 63)
    {
        pool = generate_vectors_gosper<NP>(w_u);
    }
    else
    {
        pool = generate_vectors_next_permutation<NP>(w_u);
    }

    size_t counter = 0;
    for (auto &i : pool)
    {
        Gu.Reset();
        std::cout << counter++ << ": "<<i<<"\t";
        for(int j=0; j<NP; j++)
        {
            Gu *= (i[j] == 0) ? g20 : g21;
        }
        Gu.PrintInCycleViewWithTab(std::cout);
        std::cout<<'\n';
    }
    return 0;
}

// ВЫВОДЫ:
/*
1. ЕСЛИ N < M, то утверждение подтверждается - Gu полноцикловая для всех u нечётного веса. 
*/