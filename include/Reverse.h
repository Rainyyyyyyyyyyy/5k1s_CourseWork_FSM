#ifndef REVERSE_H
#define REVERSE_H

#include "FSM.h"
#include "BinaryTree.h"
#include "Multi_Tree/MultiTree.h"


// output записан слева направо
// но f20, f21 - записаны справа налево (из-за bitset)
// решение для скорости - подавать сюда изначально развёрнутые f20, f21
// .... Строится двоичное дерево, где каждый узел имеет один ключ и до дву потомков
void Reverse_first_2powN_To_BinaryTree(const std::vector<FSM::StateNumber> &g20,
                                       const std::vector<FSM::StateNumber> &g21,
                                       const std::bitset<MAX_MP_SIZE_FOR_FSM> &f20,
                                       const std::bitset<MAX_MP_SIZE_FOR_FSM> &f21,
                                       // const std::vector<FSM::Bit> &f20,
                                       // const std::vector<FSM::Bit> &f21,
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
        if (node->chet == false)
        {
            DeleteFromLeafToUp(node);
        }
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
        Reverse_first_2powN_To_BinaryTree(g20, g21, f20, f21, output, N, M, node->left->state, node->left, i + 1);
    }
    if (node != nullptr)
        if (node->right != nullptr)
        {
            Reverse_first_2powN_To_BinaryTree(g20, g21, f20, f21, output, N, M, node->right->state, node->right, i + 1);
        }
    if (node != nullptr)
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            DeleteFromLeafToUp(node);
        }
    }
}

// output записан слева направо
// но f20, f21 - записаны справа налево (из-за bitset)
// решение для скорости - подавать сюда изначально развёрнутые f20, f21
// .... Строится многопутевое дерево, где узле имеет один ключ и мн-во потомков
void Reverse_first_2powN_To_MultiTree(const std::vector<FSM::StateNumber> &g20,
                                      const std::vector<FSM::StateNumber> &g21,
                                      const std::bitset<MAX_MP_SIZE_FOR_FSM> &f20,
                                      const std::bitset<MAX_MP_SIZE_FOR_FSM> &f21,
                                      const std::vector<FSM::Bit> &output,
                                      const size_t &N, const size_t &M,
                                      MultiTreeNode8 *&root,
                                      MultiTreeFloors &floors,
                                      MultiTreeNode8 *node,
                                      size_t i = 0,
                                      bool chet = false)
{
    (void)M;

    if (node == nullptr)
        return;

    static const size_t pow2N = (size_t)1 << N;
    if (i >= pow2N)
    {
        if (!chet)
            DeleteFromLeafToUp(node, root, floors);
        return;
    }

    if (i >= output.size() || node->key >= g20.size() || node->key >= g21.size())
        return;

    MultiTreeFloor nextFloor = 0;
    if (!TryGetNextFloor(node->floor, nextFloor))
        return;

    const FSM::StateNumber currentState = node->key;
    const MultiTreeFloor currentFloor = node->floor;
    const MultiTreeKey currentKey = node->key;

    const bool hasLeft = f20[currentState] == output[i] && g20[currentState] <= 255;
    const bool hasRight = f21[currentState] == output[i] && g21[currentState] <= 255;
    const bool reachesEnd = i + 1 >= pow2N || i + 1 >= output.size();

    MultiTreeNode8 *leftChild = nullptr;
    MultiTreeNode8 *rightChild = nullptr;
    MultiTreeKey leftKey = 0;
    MultiTreeKey rightKey = 0;

    if (hasLeft)
    {
        leftKey = static_cast<MultiTreeKey>(g20[currentState]);
        leftChild = LinkLeftByKey(node, floors, nextFloor, leftKey);
    }
    if (hasRight)
    {
        rightKey = static_cast<MultiTreeKey>(g21[currentState]);
        rightChild = LinkRightByKey(node, floors, nextFloor, rightKey);
    }

    if (leftChild != nullptr)
    {
        Reverse_first_2powN_To_MultiTree(
            g20, g21, f20, f21, output, N, M, root, floors,
            leftChild, i + 1, chet);

        const bool nodeAlive = FindNodeOnFloor(floors, currentFloor, currentKey) == node;
        const bool childAlive = FindNodeOnFloor(floors, nextFloor, leftKey) == leftChild;
        if (!nodeAlive)
            return;

        if (childAlive && leftChild->IsLeaf() &&
            (!reachesEnd || !chet) && node->right != leftChild)
        {
            if (leftChild->back1 != nullptr && leftChild->back2 != nullptr)
            {
                node->left = nullptr;
                RemoveBackPointer(node, leftChild);
            }
            else
            {
                DeleteFromLeafToUp(leftChild, root, floors);
            }
        }
    }

    if (rightChild != nullptr)
    {
        Reverse_first_2powN_To_MultiTree(
              g20, g21, f20, f21, output, N, M, root, floors,
              rightChild, i + 1, !chet);

        const bool nodeAlive = FindNodeOnFloor(floors, currentFloor, currentKey) == node;
        const bool childAlive = FindNodeOnFloor(floors, nextFloor, rightKey) == rightChild;
        if (!nodeAlive)
            return;

        if (childAlive && rightChild->IsLeaf() &&
            (!reachesEnd || chet) && node->left != rightChild)
        {
            if (rightChild->back1 != nullptr && rightChild->back2 != nullptr)
            {
                node->right = nullptr;
                RemoveBackPointer(node, rightChild);
            }
            else
            {
                DeleteFromLeafToUp(rightChild, root, floors);
            }
        }
    }
}
#endif // REVERSE_H
