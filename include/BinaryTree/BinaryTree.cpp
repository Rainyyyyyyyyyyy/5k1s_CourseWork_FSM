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



void DestroyTree(Node<FSM::StateNumber> *root) noexcept
{
    if (root == nullptr)
        return;
    DestroyTree(root->left);
    DestroyTree(root->right);
    root->left = root->right = nullptr;
    root->back = nullptr;
    delete root;
}
//

