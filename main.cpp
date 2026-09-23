#include "FSM.h"



#include <iostream>
#include "Permutation.h"
#include "BinaryTree.h"
#include "Reverse.h"

#include <bitset>


using Bit = FSM::Bit;

int main()
{

    const std::bitset<10> abo_c("1000101001");
    std::bitset<10> abo;
    abo = std::bitset<10>("1000101001");
    abo[5] = 0;
    std::cout<<abo<<'\t'<<abo.size()<<'\n';
    std::cout<<abo_c<<'\t'<<abo_c.size()<<'\n';
    for(size_t i=0; i<abo.size(); i++){
        std::cout<<abo[i]<<' ';
    }
    std::cout<<'\n';
    for(size_t i=0; i<abo_c.size(); i++){
        std::cout<<abo_c[i]<<' ';
    }
    std::cout<<'\n';
    return 0;


    // std::vector<size_t> ggg0 = {1, 2, 3, 4, 5, 6, 7, 0};
    // Permut g20(ggg0);
    // Permut g21 = g20*g20;
    // g20.Print();
    // g21.Print();
    // for(size_t i=0; i<=5; ++i)
    // {
    //     std::cout<<"------------- i = "<<i<<'\n';
    //     Permut g2i = g21^i;
    //     g2i.Print();
    // }

    // Permut Gu = ((g21^5) * g20);
    // Gu.Print();

    // return 0;
    // 6, 7, 0, 2, 1, 3, 4, 5
    //          0,1,2,3,4,5,6,7
    FSM fsm(3, {6,7,0,2,1,3,4,5},//, 8, 9, 10, 11, 12, 13, 14, 15},
               {4,5,6,0,7,2,1,3},// 8, 9, 10, 11, 12, 13, 14, 15},
               "10000001", //{1, 0, 0, 0, 0, 0, 0, 1},// 1, 1, 0, 0, 0, 1, 1, 1},
               "10000011");//{1, 0, 0, 0, 0, 0, 1, 1});// 1, 1, 1, 1, 1, 1, 1, 1});
               std::cout<<"created fsm\n";

    FSM fsm1(1, {1,0}, {0,1}, "10", "11");

    size_t m = fsm.GetM(); size_t mP = (size_t)1<<m;
    size_t n = 3; size_t nP = (size_t)1<<n;
    size_t MN = (size_t)1<<(n+m);


    std::vector<Bit> u = {0,0,1,1,1,0,0,0};//{0,1,1,1};
    fsm.output();

    
    std::vector<Bit> z;// = fsm1.ProcessWord(u);
    std::vector<FSM::StateNumber> Y;
    z = fsm1.MakeAllPeriodReturnZ(fsm, u);
    Y = fsm1.MakeAllPeriodReturnY(fsm, u);
    //z.push_back(fsm1.Get_bit_from_currentState_by_input(0));
    //z.push_back(fsm1.Get_bit_from_currentState_by_input(1));
    //z.push_back(fsm1.Get_bit_from_currentState_by_input(1));
    //z.push_back(fsm1.Get_bit_from_currentState_by_input(1));


    for(size_t i=0; i<nP; i++)std::cout<<u[i]<<' ';
    std::cout<<'\n';
    for(size_t i=0; i<mP; i++)std::cout<<"- ";
    std::cout<<'\n';

    for(size_t i = 0; i<mP; ++i)
    {
        for(size_t j=0; j<nP; ++j)
        {
            std::cout << z[i*nP + j] << ' ';
        }
        std::cout<<'\n';
    }
    for(size_t i=0; i<mP; i++)std::cout<<"- ";
    std::cout<<'\n';
    for(size_t i = 0; i<mP; ++i)
    {
        for(size_t j=0; j<nP; ++j)
        {
            std::cout << Y[i*nP + j] << ' ';
        }
        std::cout<<'\n';
    }

    Node<FSM::StateNumber> *root = new Node<FSM::StateNumber>;
    root->state = 0;
    root->left = root->right = nullptr;
    root->back = nullptr;

    // Reverse_first_2powN(fsm.Getg0(), fsm.Getg1(), fsm.Getf0(), fsm.Getf1(), z, n, root->state, root);
    // PrintTree(root);

    // fsm.ProcessWord({0, 1, 0, 1});
    DestroyTree(root);
    return 0;
    
}