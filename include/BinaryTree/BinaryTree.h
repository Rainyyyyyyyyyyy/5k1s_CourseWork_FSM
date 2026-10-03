#pragma once

#include "FSM.h"
#include <utility>
#include <iostream>
#include <string>
#include <fstream>

template <typename T>
struct Node
{
    T state;
    Node *left;
    Node *right;
    Node *back;
    bool chet = false; // чётность: true - нечётная, false - чётная
    Node(FSM::StateNumber s) : state(s), left(nullptr), right(nullptr), back(nullptr) {}
    Node() : state(0), left(nullptr), right(nullptr), back(nullptr) {}
};

template <typename T>
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
    if (parent->left != nullptr && parent->right != nullptr)
    {
        if (parent->left == node)
        {
            node->left = nullptr;
            node->right = nullptr;
            node->back = nullptr;
            delete node;
            parent->left = nullptr;
            return;
        }
        else if (parent->right == node)
        {
            node->left = nullptr;
            node->right = nullptr;
            node->back = nullptr;
            delete node;
            parent->right = nullptr;
            return;
        }
    }
    node->left = nullptr;
    node->right = nullptr;
    node->back = nullptr;
    delete node;

    DeleteFromLeafToUp(parent);
}

template <typename T>
void PrintTree(Node<T> *node, const unsigned int &shift = 2, unsigned int level = 2)
{
    if (node == nullptr)
        return;

    if (node->right != nullptr)
        PrintTree(node->right, shift, level + 1);
    std::string stroka = "";
    for (size_t i = 0; i < (level - 1) * shift - 1; i++)
        stroka = stroka + "| ";
    std::cout << stroka;
    std::cout << 'L';
    std::cout << std::string(shift, '-');
    std::cout << node->state << '\n';

    if (node->left != nullptr)
        PrintTree(node->left, shift, level + 1);
}

template <typename T>
void printTreePretty(Node<T> *root, const std::string &prefix = "", bool isLeft = true)
{
    if (!root)
        return;

    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");
    std::cout << root->state << "\n";

    // Формируем префикс для детей
    std::string childPrefix = prefix + (isLeft ? "│   " : "    ");

    // Определяем, есть ли дети (для правильного рисования)
    if (root->left || root->right)
    {
        if (root->left)
            printTreePretty(root->left, childPrefix, true);
        if (root->right)
            printTreePretty(root->right, childPrefix, false);
    }
}
template <typename T>
void printTreeLR(Node<T> *root, const std::string &prefix = "", bool isLeft = true)
{
    if (!root)
        return;

    std::cout << prefix;
    if (!prefix.empty())
    {
        std::cout << (isLeft ? "├──L " : "└──R ");
    }
    else
    {
        std::cout << "root: ";
    }
    std::cout << root->state << "\n";

    std::string childPrefix = prefix + (isLeft ? "│   " : "    ");
    if (root->left)
        printTreeLR(root->left, childPrefix, true);
    if (root->right)
        printTreeLR(root->right, childPrefix, false);
}

namespace
{
    template <typename T>
    void writeDot(Node<T> *root, std::ofstream &out)
    {
        if (!root)
            return;
        out << "    n" << root << " [label=\"" << root->state << "\"];\n";
        if (root->left)
        {
            out << "    n" << root << " -> n" << root->left << " [label=\"0\"];\n";
            writeDot(root->left, out);
        }
        if (root->right)
        {
            out << "    n" << root << " -> n" << root->right << " [label=\"1\"];\n";
            writeDot(root->right, out);
        }
    }
} // namespace

// пишет дерево в текстовый файл tree.dot
// далее необходимо выполнить dot -Tpng tree.dot -o tree.png
template <typename T>
void printTreeToFileDot(Node<T> *root, std::ofstream &out, std::string filename = "BinTree.dot")
{
    out.open(filename);
    out << "digraph Tree {\n";
    out << "    node [shape=circle];\n";
    writeDot(root, out);
    out << "}\n";
    out.close();
}




class Tree
{
private:
    Node<FSM::StateNumber> *root;

    static void Destroy(Node<FSM::StateNumber> *node) noexcept;

public:
    Tree(FSM::StateNumber rootState) : root(new Node<FSM::StateNumber>(rootState)) {}

    ~Tree() noexcept { Destroy(root); }

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