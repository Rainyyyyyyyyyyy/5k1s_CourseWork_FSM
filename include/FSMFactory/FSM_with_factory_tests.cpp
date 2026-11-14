#include "FSMFactory.h"

#include <iostream>
#include <bitset>
#include <vector>

int main()
{

    auto fsm = FSMFactory::CreateFromStringAs_f2<3>({1, 2, 3, 4, 5, 6, 7, 0},
                                                    {2, 3, 4, 5, 6, 7, 0, 1},
                                                    "10000000",
                                                    "10000011");
    fsm->output();

    size_t MP = fsm->getMP();
    constexpr size_t N = 3;
    constexpr size_t NP = static_cast<size_t>(1) << N;

    std::bitset<NP> u3("00000111");

    auto veca = fsm->MakeAllPeriodReturnY<N>(u3);
    auto vecaz = fsm->MakeAllPeriodReturnZ<N>(u3);

    std::cout<<veca.size()<<' '<<vecaz.size()<<'\n';

    for (size_t i = 0; i < MP; i++)
    {
        for (size_t j = 0; j < NP; j++)
        {
            std::cout << veca[i * NP + j] << ' ';
        }
        std::cout << '\n';
    }

    for (size_t i = 0; i < MP; i++)
    {
        for (size_t j = 0; j < NP; j++)
        {
            std::cout << vecaz[i * NP + j] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}