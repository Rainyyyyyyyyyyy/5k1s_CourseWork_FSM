#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using MultiTreeKey = std::uint8_t;
using MultiTreeFloor = std::uint8_t;

template <typename T = MultiTreeKey>
struct MultiTreeNode
{
    T key{};
    MultiTreeFloor floor = 0;
    MultiTreeNode *left = nullptr;
    MultiTreeNode *right = nullptr;
    MultiTreeNode *back1 = nullptr;
    MultiTreeNode *back2 = nullptr;

    bool IsLeaf() const noexcept;
};

using MultiTreeNode8 = MultiTreeNode<>;
using MultiTreeFloors = std::vector<std::unordered_map<MultiTreeKey, MultiTreeNode8 *>>;

MultiTreeNode8 *FindNodeOnFloor(const MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key) noexcept;

MultiTreeNode8 *GetOrCreateNode(MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key);

bool TryGetNextFloor(MultiTreeFloor current,
                     MultiTreeFloor &next) noexcept;

template <typename T>
void AddBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

template <typename T>
void RemoveBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

template <typename T>
void LinkLeft(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

template <typename T>
void LinkRight(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

MultiTreeNode8 *LinkLeftByKey(MultiTreeNode8 *parent,
                              MultiTreeFloors &floors,
                              MultiTreeFloor childFloor,
                              MultiTreeKey childKey);

MultiTreeNode8 *LinkRightByKey(MultiTreeNode8 *parent,
                               MultiTreeFloors &floors,
                               MultiTreeFloor childFloor,
                               MultiTreeKey childKey);

void RemoveNodeFromFloors(MultiTreeFloors &floors,
                          MultiTreeNode8 *node) noexcept;

namespace multi_tree_detail
{
    template <typename T>
    void PrintTreeImpl(const MultiTreeNode<T> *node,
                      const std::string &prefix,
                      const std::string &edge,
                      bool isLast,
                      std::unordered_set<const MultiTreeNode<T> *> &visited);

    template <typename T>
    void DeleteEmptyNode(MultiTreeNode<T> *node, MultiTreeNode<T> *&root);

    template <typename T>
    void WriteDot(const MultiTreeNode<T> *node,
                  std::ofstream &out,
                  std::unordered_set<const MultiTreeNode<T> *> &visited);

    void DeleteEmptyNodeWithFloors(MultiTreeNode8 *node,
                                   MultiTreeNode8 *&root,
                                   MultiTreeFloors &floors);
}

template <typename T>
void PrintTree(const MultiTreeNode<T> *node,
              const std::string &prefix = "",
              bool isLast = true);

template <typename T>
void printTreePretty(const MultiTreeNode<T> *root,
                    const std::string &prefix = "",
                    bool isLast = true);

template <typename T>
void printTreeLR(const MultiTreeNode<T> *root,
                const std::string &prefix = "",
                bool isRight = false);

template <typename T>
void printTreeToFileDot(const MultiTreeNode<T> *root,
                        std::ofstream &out,
                        const std::string &fileName = "tree.dot");

template <typename T>
void DeleteFromLeafToUp(MultiTreeNode<T> *leaf,
                       MultiTreeNode<T> *&root);

void DeleteFromLeafToUp(MultiTreeNode8 *leaf,
                        MultiTreeNode8 *&root,
                        MultiTreeFloors &floors);

template <typename T>
void DestroyMultiTree(MultiTreeNode<T> *node,
                      std::unordered_set<MultiTreeNode<T> *> &visited);

template <typename T>
void DestroyMultiTree(MultiTreeNode<T> *node) noexcept;
