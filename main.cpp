#include "FSM.h"



#include <iostream>



using Bit = FSM::Bit;

int main()
{
    FSM fsm(4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
               {1, 2, 3, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
               {0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
               {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    std::vector<Bit> u = {0,1,1,1};
    fsm.output();
    std::vector<Bit> z = fsm.ProcessWord(u);
    for(size_t i = 0; i < z.size(); ++i)
    {
        std::cout << z[i] << " ";
    }

    

    // fsm.ProcessWord({0, 1, 0, 1});

    return 0;
}