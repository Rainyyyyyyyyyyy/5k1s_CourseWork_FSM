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
    delete node;
}

//
//
//
//
//


template<typename T>
void DeletePartOfTree(Node<T> &node) noexcept
{
    if (node.left != nullptr)
    {   // слева не пусто
        DeletePartOfTree(*node.left);
        delete node.left;
        node.left = nullptr;
    }
    if (node.right != nullptr)
    {   // справа не пусто
        DeletePartOfTree(*node.right);
        delete node.right;
        node.right = nullptr;
    }
    node->back = nullptr;
    delete node;
}




