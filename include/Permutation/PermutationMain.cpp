#include "Permutation.h"


int main() {
    int N = 8;  // можно поменять

    // cycle[0] = 0 всегда; хвост перебираем
    std::vector<int> cycle(N);
    for (int i = 0; i < N; ++i) cycle[i] = i;
    // cycle = [0, 1, 2, ..., N-1]

    long long count = 0;
    do {
        std::vector<int> pi = cycle_to_perm(cycle);

        // Проверка (можно убрать — для скорости)
        /*if (!is_single_cycle(pi)) {
            std::cerr << "BUG: not a single cycle!\n";
            return 1;
        }*/

        print_perm(pi, cycle);
        ++count;

        // next_permutation по хвосту [1..N-1]
        // т.е. по cycle.begin()+1 .. cycle.end()
    } while (std::next_permutation(cycle.begin() + 1, cycle.end()));

    std::cout << "\nTotal: " << count << " (expected " << 
        [&]{ long long f = 1; for (int i = 2; i < N; ++i) f *= i; return f; }()
        << ")\n";
    return 0;
}