#include "MultiTree.h"

#include <iostream>


template <typename T>
bool MultiTreeNode<T>::IsLeaf() const noexcept
{
    return left == nullptr && right == nullptr;
}

MultiTreeNode8 *FindNodeOnFloor(const MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key) noexcept
{
    if (floor >= floors.size())
        return nullptr;

    const auto &nodes = floors[floor];
    const auto it = nodes.find(key);
    return it == nodes.end() ? nullptr : it->second;
}

MultiTreeNode8 *GetOrCreateNode(MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key)
{
    if (floors.size() <= floor)
        floors.resize(static_cast<std::size_t>(floor) + 1);

    auto &nodes = floors[floor];
    const auto it = nodes.find(key);
    if (it != nodes.end())
        return it->second;

    auto *node = new MultiTreeNode8;
    node->key = key;
    node->floor = floor;
    nodes.emplace(key, node);
    return node;
}

bool TryGetNextFloor(MultiTreeFloor current,
                     MultiTreeFloor &next) noexcept
{
    if (current == 255)
        return false;

    next = static_cast<MultiTreeFloor>(current + 1);
    return true;
}

template <typename T>
void AddBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child)
{
    if (child == nullptr || child->back1 == parent || child->back2 == parent)
        return;

    if (child->back1 == nullptr)
        child->back1 = parent;
    else if (child->back2 == nullptr)
        child->back2 = parent;
}

template <typename T>
void RemoveBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child)
{
    if (child == nullptr)
        return;

    if (child->back1 == parent)
        child->back1 = child->back2;
    if (child->back2 == parent)
        child->back2 = nullptr;
}

template <typename T>
void LinkLeft(MultiTreeNode<T> *parent, MultiTreeNode<T> *child)
{
    if (parent == nullptr)
        return;

    RemoveBackPointer(parent, parent->left);
    parent->left = child;
    AddBackPointer(parent, child);
}

template <typename T>
void LinkRight(MultiTreeNode<T> *parent, MultiTreeNode<T> *child)
{
    if (parent == nullptr)
        return;

    RemoveBackPointer(parent, parent->right);
    parent->right = child;
    AddBackPointer(parent, child);
}

MultiTreeNode8 *LinkLeftByKey(MultiTreeNode8 *parent,
                              MultiTreeFloors &floors,
                              MultiTreeFloor childFloor,
                              MultiTreeKey childKey)
{
    MultiTreeNode8 *child = GetOrCreateNode(floors, childFloor, childKey);
    LinkLeft(parent, child);
    return child;
}

MultiTreeNode8 *LinkRightByKey(MultiTreeNode8 *parent,
                               MultiTreeFloors &floors,
                               MultiTreeFloor childFloor,
                               MultiTreeKey childKey)
{
    MultiTreeNode8 *child = GetOrCreateNode(floors, childFloor, childKey);
    LinkRight(parent, child);
    return child;
}

void RemoveNodeFromFloors(MultiTreeFloors &floors,
                          MultiTreeNode8 *node) noexcept
{
    if (node == nullptr || node->floor >= floors.size())
        return;

    auto &nodes = floors[node->floor];
    const auto it = nodes.find(node->key);
    if (it != nodes.end() && it->second == node)
        nodes.erase(it);
}

namespace multi_tree_detail
{
    template <typename T>
    void PrintTreeImpl(const MultiTreeNode<T> *node,
                      const std::string &prefix,
                      const std::string &edge,
                      bool isLast,
                      std::unordered_set<const MultiTreeNode<T> *> &visited)
    {
        if (node == nullptr)
            return;

        std::cout << prefix << (isLast ? "`-- " : "|-- ") << edge
                  << static_cast<unsigned int>(node->key);
        if (!visited.insert(node).second)
        {
            std::cout << " (shared)\n";
            return;
        }
        std::cout << "\n";

        const std::string childPrefix = prefix + (isLast ? "    " : "|   ");
        if (node->left != nullptr)
            PrintTreeImpl(node->left, childPrefix, "0: ", node->right == nullptr, visited);
        if (node->right != nullptr)
            PrintTreeImpl(node->right, childPrefix, "1: ", true, visited);
    }

    template <typename T>
    void DeleteEmptyNode(MultiTreeNode<T> *node, MultiTreeNode<T> *&root)
    {
        if (node == nullptr || !node->IsLeaf())
            return;

        MultiTreeNode<T> *parent1 = node->back1;
        MultiTreeNode<T> *parent2 = node->back2;

        if (parent1 != nullptr)
        {
            if (parent1->left == node)
                parent1->left = nullptr;
            if (parent1->right == node)
                parent1->right = nullptr;
            RemoveBackPointer(parent1, node);
        }
        if (parent2 != nullptr && parent2 != parent1)
        {
            if (parent2->left == node)
                parent2->left = nullptr;
            if (parent2->right == node)
                parent2->right = nullptr;
            RemoveBackPointer(parent2, node);
        }

        node->back1 = nullptr;
        node->back2 = nullptr;
        if (node == root)
            root = nullptr;
        delete node;

        if (parent1 != nullptr && parent1->IsLeaf())
            DeleteEmptyNode(parent1, root);
        if (parent2 != nullptr && parent2 != parent1 && parent2->IsLeaf())
            DeleteEmptyNode(parent2, root);
    }

    template <typename T>
    void WriteDot(const MultiTreeNode<T> *node,
                  std::ofstream &out,
                  std::unordered_set<const MultiTreeNode<T> *> &visited)
    {
        if (node == nullptr || !visited.insert(node).second)
            return;

        out << "    n" << node << " [label=\""
            << static_cast<unsigned int>(node->key) << "\"];\n";
        if (node->left != nullptr)
        {
            out << "    n" << node << " -> n" << node->left
                << " [label=\"0\"];\n";
            WriteDot(node->left, out, visited);
        }
        if (node->right != nullptr)
        {
            out << "    n" << node << " -> n" << node->right
                << " [label=\"1\"];\n";
            WriteDot(node->right, out, visited);
        }
    }

    void DeleteEmptyNodeWithFloors(MultiTreeNode8 *node,
                                   MultiTreeNode8 *&root,
                                   MultiTreeFloors &floors)
    {
        if (node == nullptr || !node->IsLeaf())
            return;

        MultiTreeNode8 *parent1 = node->back1;
        MultiTreeNode8 *parent2 = node->back2;

        if (parent1 != nullptr)
        {
            if (parent1->left == node)
                parent1->left = nullptr;
            if (parent1->right == node)
                parent1->right = nullptr;
            RemoveBackPointer(parent1, node);
        }
        if (parent2 != nullptr && parent2 != parent1)
        {
            if (parent2->left == node)
                parent2->left = nullptr;
            if (parent2->right == node)
                parent2->right = nullptr;
            RemoveBackPointer(parent2, node);
        }

        RemoveNodeFromFloors(floors, node);
        node->back1 = nullptr;
        node->back2 = nullptr;
        if (node == root)
            root = nullptr;
        delete node;

        if (parent1 != nullptr && parent1->IsLeaf())
            DeleteEmptyNodeWithFloors(parent1, root, floors);
        if (parent2 != nullptr && parent2 != parent1 && parent2->IsLeaf())
            DeleteEmptyNodeWithFloors(parent2, root, floors);
    }
} // namespace multi_tree_detail

template <typename T>
void PrintTree(const MultiTreeNode<T> *node,
              const std::string &prefix,
              bool isLast)
{
    if (node == nullptr)
        return;

    std::unordered_set<const MultiTreeNode<T> *> visited;
    multi_tree_detail::PrintTreeImpl(node, prefix, "", isLast, visited);
}

template <typename T>
void printTreePretty(const MultiTreeNode<T> *root,
                    const std::string &prefix,
                    bool isLast)
{
    if (root == nullptr)
        return;

    std::unordered_set<const MultiTreeNode<T> *> visited;
    const auto print = [&](const auto &self,
                           const MultiTreeNode<T> *node,
                           const std::string &currentPrefix,
                           bool last) -> void
    {
        if (node == nullptr)
            return;

        std::cout << currentPrefix << (last ? "└── " : "├── ")
                  << static_cast<unsigned int>(node->key);
        if (!visited.insert(node).second)
        {
            std::cout << " (shared)\n";
            return;
        }
        std::cout << "\n";

        const std::string childPrefix = currentPrefix + (last ? "    " : "│   ");
        if (node->left != nullptr)
            self(self, node->left, childPrefix, node->right == nullptr);
        if (node->right != nullptr)
            self(self, node->right, childPrefix, true);
    };

    print(print, root, prefix, isLast);
}

template <typename T>
void printTreeLR(const MultiTreeNode<T> *root,
                const std::string &prefix,
                bool isRight)
{
    if (root == nullptr)
        return;

    if (prefix.empty())
        std::cout << "root: " << static_cast<unsigned int>(root->key) << "\n";
    else
        std::cout << prefix << (isRight ? "`--1 " : "|--0 ")
                  << static_cast<unsigned int>(root->key) << "\n";

    const std::string childPrefix = prefix + (prefix.empty() ? "" : "|   ");
    if (root->left != nullptr)
        printTreeLR(root->left, childPrefix, false);
    if (root->right != nullptr)
        printTreeLR(root->right, childPrefix, true);
}

template <typename T>
void printTreeToFileDot(const MultiTreeNode<T> *root,
                        std::ofstream &out,
                        const std::string &fileName)
{
    out.open(fileName);
    out << "digraph Tree {\n";
    out << "    node [shape=circle];\n";
    std::unordered_set<const MultiTreeNode<T> *> visited;
    multi_tree_detail::WriteDot(root, out, visited);
    out << "}\n";
    out.close();
}

template <typename T>
void DeleteFromLeafToUp(MultiTreeNode<T> *leaf,
                       MultiTreeNode<T> *&root)
{
    if (leaf == nullptr || root == nullptr || !leaf->IsLeaf())
        return;

    multi_tree_detail::DeleteEmptyNode(leaf, root);
}

void DeleteFromLeafToUp(MultiTreeNode8 *leaf,
                        MultiTreeNode8 *&root,
                        MultiTreeFloors &floors)
{
    if (leaf == nullptr || root == nullptr || !leaf->IsLeaf())
        return;

    multi_tree_detail::DeleteEmptyNodeWithFloors(leaf, root, floors);
}

template <typename T>
void DestroyMultiTree(MultiTreeNode<T> *node,
                      std::unordered_set<MultiTreeNode<T> *> &visited)
{
    if (node == nullptr || !visited.insert(node).second)
        return;

    DestroyMultiTree(node->left, visited);
    DestroyMultiTree(node->right, visited);
    delete node;
}

template <typename T>
void DestroyMultiTree(MultiTreeNode<T> *node) noexcept
{
    std::unordered_set<MultiTreeNode<T> *> visited;
    DestroyMultiTree(node, visited);
}

template struct MultiTreeNode<MultiTreeKey>;
template void AddBackPointer<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, MultiTreeNode<MultiTreeKey> *);
template void RemoveBackPointer<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, MultiTreeNode<MultiTreeKey> *);
template void LinkLeft<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, MultiTreeNode<MultiTreeKey> *);
template void LinkRight<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, MultiTreeNode<MultiTreeKey> *);
template void PrintTree<MultiTreeKey>(const MultiTreeNode<MultiTreeKey> *, const std::string &, bool);
template void printTreePretty<MultiTreeKey>(const MultiTreeNode<MultiTreeKey> *, const std::string &, bool);
template void printTreeLR<MultiTreeKey>(const MultiTreeNode<MultiTreeKey> *, const std::string &, bool);
template void printTreeToFileDot<MultiTreeKey>(const MultiTreeNode<MultiTreeKey> *, std::ofstream &, const std::string &);
template void DeleteFromLeafToUp<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, MultiTreeNode<MultiTreeKey> *&);
template void DestroyMultiTree<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *, std::unordered_set<MultiTreeNode<MultiTreeKey> *> &);
template void DestroyMultiTree<MultiTreeKey>(MultiTreeNode<MultiTreeKey> *);
