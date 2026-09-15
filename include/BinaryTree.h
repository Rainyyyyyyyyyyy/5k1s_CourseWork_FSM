#pragma once

#include "FSM.h"
#include "BinaryTree.h"
#include <utility>

template <typename T>
struct Node
{
    T state;
    Node *left;
    Node *right;
    Node *back;
    Node(FSM::StateNumber s) : state(s), left(nullptr), right(nullptr), back(nullptr) {}
    Node() : state(0), left(nullptr), right(nullptr), back(nullptr) {}
};

template<typename T>
void DeletePartOfTree(Node<T> &node) noexcept;


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
    void GetLeaves(Node<FSM::StateNumber> *node, std::vector<FSM::StateNumber> &leaves) const;
};

