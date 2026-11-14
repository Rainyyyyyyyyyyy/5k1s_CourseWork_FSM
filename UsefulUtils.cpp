// Рекурсивный перебор (n - 1)! полноцикловых перестановок с фиксацией нуля
#include <vector>
#include <unordered_set>
#include <cmath>

std::vector<int> pi;
std::unordered_set<int> used;
void backtrack(int current_vertex, int step)
{
    if (step == n)
    {
        pi[current_vertex] = 0; // Замыкаем цикл обратно в ноль
        // processCandidatePermutation();
        // std::cout<<"=========================\n";
        return;
    }
    for (int next_vertex = 1; next_vertex < n; ++next_vertex)
    {
        if (used.find(next_vertex) == used.end())
        { // if (used[next_vertex] == false) {
            // std::cout<<"next: "<<next_vertex<<"      <-    "<<"current: "<<current_vertex<<'\n';
            pi[current_vertex] = next_vertex;
            used[next_vertex] = true;

            backtrack(next_vertex, step + 1);

            used.erase(next_vertex); // used[next_vertex] = false; // Шаг назад
        }
    }
}

/*
 * pool = набор векторов веса right_steps
 * left_steps = кол-во нулей в векторе
 * right_steps = кол-во единиц в векторе
 */
namespace
{

    struct BinaryTree
    {
        BinaryTree *left;
        BinaryTree *right;
        size_t data;

        // удаление дерева
        void delete_tree(BinaryTree *cur_root)
        {
            if (cur_root == nullptr)
                return;

            delete_tree(cur_root->left);
            delete_tree(cur_root->right);

            left = right = nullptr;
            delete cur_root;
        }

        // конструктор
        BinaryTree(BinaryTree *t_left = nullptr, BinaryTree *t_right = nullptr, const size_t &t_data = 0)
            : left(t_left), right(t_right), data(t_data) {}

        // деструктор
        ~BinaryTree()
        {
            delete_tree(this);
        }
    };

    inline double factorial(const int &n)
    {
        return std::tgamma(n + 1);
    }

    inline size_t C_n_k(const size_t &n, const size_t &k)
    {
        return (static_cast<size_t>(factorial(n))) /
               (static_cast<size_t>(factorial(k)) * static_cast<size_t>(factorial(n - k)));
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
                new_right->data = temp = cur_root->data | (1 << (WALL - deep - 1));
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
            BinaryTree *tree = new BinaryTree();
            std::vector<size_t> pool_with_i_weight;
            make_pool_fixed_weight(tree, pool_with_i_weight,
                                   lenght - i, i, lenght);

            pool.insert(pool.end(),
                        std::make_move_iterator(pool_with_i_weight.begin()),
                        std::make_move_iterator(pool_with_i_weight.end()));
        }
    }





} // namespace


// алгоритм Госпера для генерации всех векторов длины N и веса k
template <size_t N>
std::vector<std::bitset<N>> generate_vectors(size_t k) {
    static_assert(N <= 63, "Для N > 63 стандартного uint64_t не хватит (нужен __uint128_t)");
    
    std::vector<std::bitset<N>> result;
    
    // Если вес больше длины, таких векторов не существует
    if (k > N) {
        return result;
    }
    
    // Крайний случай: вес 0 (только нулевой вектор)
    if (k == 0) {
        result.emplace_back(0);
        return result;
    }

    uint64_t sub = (1ULL << k) - 1;
    uint64_t limit = 1ULL << N;

    while (sub < limit) {
        result.emplace_back(sub);

        // Алгоритм Госпера
        uint64_t c = sub & -sub;
        uint64_t r = sub + c;
        sub = (((r ^ sub) >> 2) / c) | r;
    }

    return result;
}





//
//
//

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


    // вычисление биномиального коэффициента
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