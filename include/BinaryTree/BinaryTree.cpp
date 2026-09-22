#include "BinaryTree.h"

void Tree::GetLeaves(Node<FSM::StateNumber> *node, std::vector<FSM::StateNumber> &leaves) const
{
    if (node == nullptr)
    {
        return;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        leaves.push_back(node->state);
        return;
    }

    GetLeaves(node->left, leaves);
    GetLeaves(node->right, leaves);
}

Tree &Tree::operator=(Tree &&other) noexcept
{
    if (this != &other)
    {
        Destroy(root);
        root = std::exchange(other.root, nullptr);
    }
    return *this;
}

void Tree::Destroy(Node<FSM::StateNumber> *node) noexcept
{
    if (node == nullptr)
        return;
    Destroy(node->left);
    Destroy(node->right);
    node->left = node->right = nullptr;
    node->back = nullptr;
    delete node;
}

//
//
//
//
//

template <typename T>
void DeletePartOfTree(Node<T> *node) noexcept
{
    if (node->left != nullptr)
    { // слева не пусто
        DeletePartOfTree(node->left);
        delete node->left;
        node->left = nullptr;
    }
    if (node->right != nullptr)
    { // справа не пусто
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
    if (node->back == nullptr)
    { // если дошли до корня
        node->left = node->right = nullptr;
        node->back = nullptr;
        delete node;
        return;
    }
    if (node->back->left != nullptr && node->back->right != nullptr)
    { // если у родителя есть два потомка
        node->left = node->right = nullptr;
        node->back = nullptr;
        delete node;
        return;
    }
    Node<T> *parent = node->back;
    node->left = node->right = nullptr;
    node->back = nullptr;
    delete node;
    DeleteFromLeafToUp(parent);
}
