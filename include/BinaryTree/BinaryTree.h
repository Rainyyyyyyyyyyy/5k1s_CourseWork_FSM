#pragma once

#include "FSM.h"
#include <utility>
#include <iostream>
#include <string>

template <typename T>
struct Node
{
    T state;
    Node *left;
    Node *right;
    Node *back;
    bool chet = false;  // чётность: true - нечётная, false - чётная
    Node(FSM::StateNumber s) : state(s), left(nullptr), right(nullptr), back(nullptr) {}
    Node() : state(0), left(nullptr), right(nullptr), back(nullptr) {}
};

template<typename T>
void DeletePartOfTree(Node<T> *node) noexcept
{
    if (node == nullptr)
        return;

    if (node->left != nullptr)
    {
        DeletePartOfTree(node->left);
        delete node->left;
        node->left = nullptr;
    }

    if (node->right != nullptr)
    {
        DeletePartOfTree(node->right);
        delete node->right;
        node->right = nullptr;
    }

    node->back = nullptr;
    delete node;
}

// удаление ветви дерева от листа вверх, до узла с двумя потомками (или до корня, если такового нет)
template <typename T>
void DeleteFromLeafToUp(Node<T> *node)
{
    if (node == nullptr)
        return;

    Node<T> *parent = node->back;

    if (parent == nullptr)
        return;

    if (parent->left == node)
        parent->left = nullptr;
    else if (parent->right == node)
        parent->right = nullptr;

    node->left = nullptr;
    node->right = nullptr;
    node->back = nullptr;
    delete node;

    if (parent->left != nullptr && parent->right != nullptr)
        return;

    DeleteFromLeafToUp(parent);
}

template <typename T>
void PrintTree(Node<T> *node, const unsigned int &shift = 2, unsigned int level = 0)
{
    if (node == nullptr)
        return;

    if (node->right != nullptr)
        PrintTree(node->right, shift, level + 1);

    std::cout << std::string(level * shift, ' ');
    std::cout << node->state << '\n';

    if (node->left != nullptr)
        PrintTree(node->left, shift, level + 1);
}

class Tree
{
private:
    Node<FSM::StateNumber> *root;

    static void Destroy(Node<FSM::StateNumber> *node) noexcept;

public:
    Tree(FSM::StateNumber rootState) : root(new Node<FSM::StateNumber>(rootState)) {}

    ~Tree() noexcept{ Destroy(root); }

    Tree(const Tree &) = delete;
    Tree &operator=(const Tree &) = delete;

    Tree(Tree &&other) noexcept : root(std::exchange(other.root, nullptr)) {}

    Tree &operator=(Tree &&other) noexcept;

    Node<FSM::StateNumber> *GetRoot() const noexcept { return root; }
    FSM::StateNumber GetRootState() const noexcept { return root->state; }

    // проходит дерево вглубь и записывает значения листьев в leaves
    void GetLeaves(Node<FSM::StateNumber> *node, std::vector<FSM::StateNumber> &leaves) const;

};


// публичный безклассовый метод удаления дерева
void DestroyTree(Node<FSM::StateNumber> *root) noexcept;