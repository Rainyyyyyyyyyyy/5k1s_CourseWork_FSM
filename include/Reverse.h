#ifndef REVERSE_H
#define REVERSE_H

#include "FSM.h"
#include "BinaryTree.h"


// output записан слева направо
// но f20, f21 - записаны справа налево (из-за bitset)
// решение для скорости - подавать сюда изначально развёрнутые f20, f21
void Reverse_first_2powN(const std::vector<FSM::StateNumber> &g20,
                         const std::vector<FSM::StateNumber> &g21,
                         const std::bitset<MAX_MP_SIZE_FOR_FSM> &f20,
                         const std::bitset<MAX_MP_SIZE_FOR_FSM> &f21,
                         //const std::vector<FSM::Bit> &f20,
                         //const std::vector<FSM::Bit> &f21,
                         const std::vector<FSM::Bit> &output,
                         const size_t &N, const size_t &M, 
                         size_t &newState,
                         Node<FSM::StateNumber> *node, size_t i = 0)
{
    if (node == nullptr)
        return;

    static const size_t pow2N = (size_t)1 << N;
    static const size_t pow2M = (size_t)1 << M;
    if (i >= pow2N)
        {
            //if(node->chet == true){
            //    DeleteFromLeafToUp(node);
            //}
            return;
        }

    if (f20[newState] == output[i])
    {
        Node<FSM::StateNumber> *leftNode = new Node<FSM::StateNumber>;
        node->left = leftNode;
        leftNode->state = g20[newState];
        leftNode->back = node;
        leftNode->chet = node->chet;
        leftNode->left = leftNode->right = nullptr;
    }

    if (f21[newState] == output[i])
    {
        Node<FSM::StateNumber> *rightNode = new Node<FSM::StateNumber>;
        node->right = rightNode;
        rightNode->state = g21[newState];
        rightNode->back = node;
        rightNode->chet = !node->chet;
        rightNode->left = rightNode->right = nullptr;
    }

    if (node->left != nullptr)
    {
        Reverse_first_2powN(g20, g21, f20, f21, output, N, M, node->left->state, node->left, i + 1);
    }
    if(node!= nullptr)
    if (node->right != nullptr)
    {
        Reverse_first_2powN(g20, g21, f20, f21, output, N, M, node->right->state, node->right, i + 1);
    }
    if(node != nullptr){
        if(node->left == nullptr && node->right == nullptr){
            DeleteFromLeafToUp(node);
        }
    }

}

#endif // REVERSE_H
