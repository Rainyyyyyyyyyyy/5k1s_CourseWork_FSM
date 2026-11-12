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
                                      std::vector<FSM::StateNumber> &LastStates,
                                      std::vector<std::vector<FSM::Bit>> &candidates_u,
                                      std::vector<FSM::Bit> &current_u,
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
        {
            DeleteFromLeafToUp(node, root, floors);
        }
        else
        {
            LastStates.push_back(node->key);
            candidates_u.push_back(current_u);
        }
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
        current_u.push_back(false);
        Reverse_first_2powN_To_MultiTree(
            g20, g21, f20, f21, output, N, M, root, floors,
            leftChild, LastStates, candidates_u, current_u, i + 1, chet);
        current_u.pop_back();

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
        current_u.push_back(true);
        Reverse_first_2powN_To_MultiTree(
            g20, g21, f20, f21, output, N, M, root, floors,
            rightChild, LastStates, candidates_u, current_u, i + 1, !chet);
        current_u.pop_back();

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

template <typename T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Проверяет кандидатов в u(t) на предмет выработки z(t)
// &States - состояния в листьях дерева обращения
// &candidates_u - траектории от корня до листьев - кандидаты u(t)
// &output - выходная последовательность z(t)
// &g20, &g21 - таблицы переходов автомата
// &f20, &f21 - таблицы выходов автомата
// &n, &m - параметры размера
// bool start_y_is_correct - флаг верности начального состояния y(0)
//--------------------
// start_y_is_correct == true |=> возвращаем расстояние единственности d, если сократили мн-во кандидатов до 1
// start_y_is_correct == false |=> возвращаем расстояние единственности d, если сократили мн-во кандидатов до 0
size_t CheckCandidatesY_by_z(std::vector<FSM::StateNumber> &States,
                            std::vector<std::vector<FSM::Bit>> &candidates_u,
                            const std::vector<FSM::Bit> &output,
                            const std::vector<FSM::StateNumber> &g20,
                            const std::vector<FSM::StateNumber> &g21,
                            const std::bitset<MAX_MP_SIZE_FOR_FSM> &f20,
                            const std::bitset<MAX_MP_SIZE_FOR_FSM> &f21,
                            const size_t &n, const size_t &m, bool start_y_is_correct = true)
{
    size_t d = (size_t)1 << n; // расстояние единственности
    static const size_t nP = (size_t)1 << n;
    static const size_t nP_mask = nP - 1;
    size_t z_bit = nP - 1;
    static const size_t z_size = (size_t)1 << (n + m);
    size_t candsSize = candidates_u.size();
    if (candsSize == 0)
        return d;
    FSM::Bit currentBit = 0;
    // цикл по всей z(t)
    for (size_t z_bit = nP; z_bit < z_size; z_bit++)
    { d++;
        // цикл по всем кандидатам из U в u(t)
        for (size_t i = 0; i < candsSize; i++)
        {
            // цикл по длине u(t)
            // for(size_t j=0; j<nP; j++){//for(int j=nP-1; j>=0; j--){// for(size_t j=0; j<nP; j++){
            currentBit = candidates_u[i][z_bit & nP_mask];
            if (currentBit == 0)
            {
                if (f20[States[i]] != output[z_bit])
                {
                    candsSize--;
                    swap(candidates_u[i], candidates_u[candsSize]);
                    candidates_u.pop_back();
                    Swap(States[i], States[candsSize]);
                    States.pop_back();
                    i--;
                    continue;
                    // break;
                }
                States[i] = g20[States[i]];
            }
            else
            {
                if (f21[States[i]] != output[z_bit])
                {
                    candsSize--;
                    swap(candidates_u[i], candidates_u[candsSize]);
                    candidates_u.pop_back();
                    Swap(States[i], States[candsSize]);
                    States.pop_back();
                    i--;
                    continue;
                    // break;
                }
                States[i] = g21[States[i]];
            }
            // z_bit++;
        }
        candsSize = candidates_u.size();
        if (candsSize == start_y_is_correct)
            return d;
        //else
        //    d++;
    }
    return ((size_t)1 << (n + m)) + 2;
}
#endif // REVERSE_H
