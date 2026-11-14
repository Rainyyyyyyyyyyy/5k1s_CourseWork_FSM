#include "FSM.h"

#include <iostream>
#include <fstream>

#include "Permutation.h"
#include "BinaryTree.h"
#include "Reverse.h"

#include <bitset>

#include "MultiTree.h"
#include "FSMFactory.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <chrono>




int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Переключаем вывод консоли на UTF-8
#endif



    size_t n, nP, m, mP, MN; //, MNP;
    // размер n=m=4
    size_t m4 = 4;
    size_t mP4 = (size_t)1 << m4;
    size_t n4 = 4;
    size_t nP4 = (size_t)1 << n4;
    size_t MN44 = (size_t)1 << (n4 + m4);

    // размер m=n=3
    size_t m3 = 3;
    size_t mP3 = (size_t)1 << m3;
    size_t n3 = 3;
    size_t nP3 = (size_t)1 << n3;
    size_t MN33 = (size_t)1 << (n3 + m3);

    // размер m=n=2
    size_t m2 = 2;
    size_t mP2 = (size_t)1 << m2;
    size_t n2 = 2;
    size_t nP2 = (size_t)1 << 2;
    size_t MN22 = (size_t)1 << (n2 + m2);

    // размер m=n=1
    size_t m1 = 1;
    size_t mP1 = (size_t)1 << m1;
    size_t n1 = 1;
    size_t nP1 = (size_t)1 << 1;
    size_t MN11 = (size_t)1 << (n1 + m1);

    // n = 6
    size_t n6 = 6;
    //
    // АКТУАЛЬНЫЕ ПАРАМЕТРЫ (ТЕКУЩИЕ)
    //
    n = n6;
    nP = 1<<n;;
    m = m4;
    mP = 1<<m;
    //MN = MN22; // MNP = MNP22;

    //          0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    Permut g21({1,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15}); //{0,1,2,3,4,5,6,7});//{3,2,0,1});//({2,3,1,7,5,0,4,6});//{0,3,1,2});//{2,0,3,1});//{2,4,1,6,3,7,5,0});//{3,4,5,6,7,0,1,2});
    size_t deg = 6;
    //          0,1, 2,3,4, 5, 6,7,8,9,10,11,12,13,14,15
    Permut g20({3,6,12,7,5,15,14,2,1,8, 9,10, 4,11,13,0});
            // (0 3 7 2 12 4 5 15) (1 6 14 13 11 10 9 8)
    // g20.Print();
    // g21.Print();
    // return 0;
    // if(start_state >= g20.GetSize()){
    //     throw std::invalid_argument("start state is incorrect!\n Shutting down...\n");
    // }

    FSM<4> fsm34(g20.Get(), g21.Get(),
                "0000111110000000", //"1110001110001110",
                "1111011011001111"); //"0101111111001001",0);

    auto current_fsm = fsm34;
    std::cout << "created fsm\n";
    current_fsm.output();
    // FSM fsm22(2, {1, 2, 3, 0},
    //           {2, 3, 0, 1},
    //           "1000",
    //           "1100");

    // FSM fsm1(1, {1, 0}, {0, 1}, "10", "11");

    std::bitset< 1<<1 > u1("01");  
    std::bitset< 1<<2 > u2("0001");  
    std::bitset< 1<<3 > u3("00000001");  
    std::bitset< 1<<4 > u4("0000000000000001");
    std::bitset< 1<<5 > u5("00000000000000000000000000000001");  
    std::bitset< 1<<6 > u6("0000000000000000000000000000000000000000000000000000000000000001");
    auto current_u = u5;
    // fsm33.output();
    //fsm22.output();
    Permut Composition_Gu = (current_u[0] == 0) ? current_fsm.Getg0() : current_fsm.Getg1();
    // std::cout<<"current_u["<<0<<"] = "<<current_u[0]<<'\n';
    //     std::cout<<"Composition Gu: \n";
    //     Composition_Gu.Print();
    for(size_t i=1; i<current_u.size(); i++){
        Composition_Gu *= (current_u[i]==0) ? current_fsm.Getg0() : current_fsm.Getg1();
        // std::cout<<"current_u["<<i<<"] = "<<current_u[i]<<'\n';
        // std::cout<<"Composition Gu: \n";
        // Composition_Gu.Print();
    }

    std::cout<<"Composition Gu: \n";
    Composition_Gu.Print();


    std::vector<Bit> z; // = fsm1.ProcessWord(u);
    std::vector<FSM::StateNumber> Y;
    z = current_fsm.MakeAllPeriodReturnZ(current_u);
    Y = current_fsm.MakeAllPeriodReturnY(current_u);



    ///
    /// Вывод в консоль матриц-траекторий выходов z(t) и состояний y(t)
    ///
    for (size_t i = 0; i < nP; i++)std::cout << current_u[i] << '\t';   std::cout << '\n';
    for (size_t i = 0; i < mP; i++)std::cout << "-\t";           std::cout << '\n';
    for (size_t i = 0; i < mP; ++i)
    {
        for (size_t j = 0; j < nP; ++j)
        {
            std::cout << z[i * nP + j] << '\t';//'/'<<Y[i * nP + j]<<' ';
        }
        std::cout << '\n';
    }
    for (size_t i = 0; i < mP; i++) std::cout << "-\t";           std::cout << '\n';
    for (size_t i = 0; i < mP; ++i)
    {
        for (size_t j = 0; j < nP; ++j)
        {
            std::cout << Y[i * nP + j] << '\t';
        }
        std::cout << '\n';
    }

    //
    //  Reverse into Binary Tree
    //
    Node<FSM::StateNumber> *root = new Node<FSM::StateNumber>;
    root->state = 0;
    root->left = root->right = nullptr;
    root->back = nullptr;




    Reverse_first_2powN_To_BinaryTree(current_fsm.Getg0(), current_fsm.Getg1(),
                                      current_fsm.Getf0(), current_fsm.Getf1(),
                                      z, n, m,
                                      root->state, root);

    std::ofstream out;
    printTreeToFileDot(root, out, "binary_tree.dot");
    out.close();
    printTreeLR(root);
    DestroyTree(root);

    //
    // Reverse into MultiTree (DAG)
    //
    MultiTreeFloors floors;
    MultiTreeNode8 *rootMulti = GetOrCreateNode(
        floors,
        static_cast<MultiTreeFloor>(0),
        static_cast<MultiTreeKey>(0));
    std::vector<FSM::StateNumber> lastStates;
    std::vector<std::vector<FSM::Bit>> candidatesU;
    std::vector<FSM::Bit> currentU;

    Reverse_first_2powN_To_MultiTree(
        current_fsm.Getg0(), current_fsm.Getg1(),
        current_fsm.Getf0(), current_fsm.Getf1(),
        z, n, m,
        rootMulti, floors, rootMulti,
        lastStates, candidatesU, currentU);
    for(size_t i=0; i<lastStates.size(); ++i)
    {
        std::cout<<"lastStates["<<i<<"] = "<<lastStates[i]<<":  u(t) =  ";
        for(size_t j=0; j<candidatesU[i].size(); ++j)
        {
            std::cout<<candidatesU[i][j];
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';

    size_t d = CheckCandidatesY_by_z(lastStates, candidatesU, z,
                         current_fsm.Getg0(), current_fsm.Getg1(),
                         current_fsm.Getf0(), current_fsm.Getf1(),
                         n, m);
    std::cout<<"d = "<<d<<'\n';
    std::cout<<"True u(t): ";
    if(candidatesU.size() == 0)std::cout<<"|U| = 0"; else
    for (size_t i=0; i<nP; i++)
        std::cout << candidatesU[0][i];
    std::cout<<'\n';
     for(size_t i=0; i<lastStates.size(); ++i)
    {
        std::cout<<"lastStates["<<i<<"] = "<<lastStates[i]<<":  u(t) =  ";
        for(size_t j=0; j<candidatesU[i].size(); ++j)
        {
            std::cout<<candidatesU[i][j];
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
    //printTreePretty(rootMulti);

    std::ofstream outMulti;
    printTreeToFileDot(rootMulti, outMulti, "multi_tree.dot");
    outMulti.close();

    DestroyMultiTree(rootMulti);
    rootMulti = nullptr;
    floors.clear();

    return 0;
}

// тестирование перестановок
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