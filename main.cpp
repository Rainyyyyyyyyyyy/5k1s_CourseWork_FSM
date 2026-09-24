#include "FSM.h"

#include <iostream>
#include <fstream>

#include "Permutation.h"
#include "BinaryTree.h"
#include "Reverse.h"

#include <bitset>

#include "MultiTree.h"

#ifdef _WIN32
#include <windows.h>
#endif

using Bit = FSM::Bit;

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Переключаем вывод консоли на UTF-8
#endif


    size_t n, nP, m, mP, MN; //, MNP;

    // размер m=n=3
    size_t m3 = 3;
    size_t mP3 = (size_t)1 << m3;
    size_t n3 = 3;
    size_t nP3 = (size_t)1 << 3;
    size_t MN33 = (size_t)1 << (n3 + m3);

    // размер m=n=2
    size_t m2 = 2;
    size_t mP2 = (size_t)1 << m2;
    size_t n2 = 2;
    size_t nP2 = (size_t)1 << 2;
    size_t MN22 = (size_t)1 << (n2 + m2);

    //
    // АКТУАЛЬНЫЕ ПАРАМЕТРЫ (ТЕКУЩИЕ)
    //
    n = n3;
    nP = nP3;
    m = m3;
    mP = mP3;
    MN = MN33; // MNP = MNP22;

    //             0 1 2 3 4 5 6 7
    FSM fsm33(m3,
    /*g20*/         {6,7,0,2,1,3,4,5}, //, 8, 9, 10, 11, 12, 13, 14, 15},
    /*g21*/         {7,2,1,4,3,6,5,0},     //{4,5,6,0,7,2,1,3}, //{4,5,6,0,7,2,1,3},// 8, 9, 10, 11, 12, 13, 14, 15},
    /*f20*/         "10000001",                   //{1, 0, 0, 0, 0, 0, 0, 1},// 1, 1, 0, 0, 0, 1, 1, 1},
    /*f21*/         "10000000");                  //{1, 0, 0, 0, 0, 0, 1, 1});// 1, 1, 1, 1, 1, 1, 1, 1});
    std::cout << "created fsm\n";

    FSM fsm22(2, {1, 2, 3, 0},
              {2, 3, 0, 1},
              "1000",
              "1100");

    FSM fsm1(1, {1, 0}, {0, 1}, "10", "11");

    std::vector<Bit> u3 = {0, 0, 1, 1, 1, 0, 0, 0}; //{0,1,1,1};
    std::vector<Bit> u2 = {0, 1, 1, 1};
    // fsm33.output();
    fsm22.output();

    std::vector<Bit> z; // = fsm1.ProcessWord(u);
    std::vector<FSM::StateNumber> Y;
    z = fsm33.MakeAllPeriodReturnZ(u3);
    Y = fsm33.MakeAllPeriodReturnY(u3);



    ///
    /// Вывод в консоль матриц-траекторий выходов z(t) и состояний y(t)
    ///
    for (size_t i = 0; i < nP; i++)std::cout << u3[i] << ' ';   std::cout << '\n';
    for (size_t i = 0; i < mP; i++)std::cout << "- ";           std::cout << '\n';
    for (size_t i = 0; i < mP; ++i)
    {
        for (size_t j = 0; j < nP; ++j)
        {
            std::cout << z[i * nP + j] << ' ';
        }
        std::cout << '\n';
    }
    for (size_t i = 0; i < mP; i++) std::cout << "- ";           std::cout << '\n';
    for (size_t i = 0; i < mP; ++i)
    {
        for (size_t j = 0; j < nP; ++j)
        {
            std::cout << Y[i * nP + j] << ' ';
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

    Reverse_first_2powN_To_BinaryTree(fsm33.Getg0(), fsm33.Getg1(),
                                      fsm33.Getf0(), fsm33.Getf1(),
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

    Reverse_first_2powN_To_MultiTree(
        fsm33.Getg0(), fsm33.Getg1(),
        fsm33.Getf0(), fsm33.Getf1(),
        z, n, m,
        rootMulti, floors, rootMulti);

    printTreePretty(rootMulti);

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