#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using MultiTreeKey = std::uint8_t;      // 1 байт (8 бит) на номер состояния
using MultiTreeFloor = std::uint8_t;    // 1 байт (8 бит) на номер этажа
// то есть 2^n <= 2^8 и 2^m <= 2^8

template <typename T = MultiTreeKey>
struct MultiTreeNode
{
    T key{};                                // ключевое значение
    MultiTreeFloor floor = 0;               // номер этажа, на котором расположен данный узел
    MultiTreeNode *left = nullptr;          // потомок налево 
    MultiTreeNode *right = nullptr;         // потомок направо 
    MultiTreeNode *back1 = nullptr;         // родитель1
    MultiTreeNode *back2 = nullptr;         // родитель2

    // Проверяет, является ли узел листом, то есть не имеет левого и правого потомка.
    // Возвращает true для листа и false, если у узла есть хотя бы один потомок.
    inline bool IsLeaf() const noexcept;
};

using MultiTreeNode8 = MultiTreeNode<>;
using MultiTreeFloors = std::vector<std::unordered_map<MultiTreeKey, MultiTreeNode8 *>>;

// Ищет узел с заданным ключом key на указанном этаже под номером floor.
// На вход получает таблицу этажей floors, номер этажа floor и ключ узла key.
// Возвращает указатель на найденный узел или nullptr, если этаж либо узел отсутствует.
MultiTreeNode8 *FindNodeOnFloor(const MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key) noexcept;

// Возвращает существующий узел с заданным ключом или создает новый.
// На вход получает таблицу этажей, номер этажа и ключ узла.
// При необходимости расширяет таблицу, создает узел и возвращает указатель на него.
MultiTreeNode8 *GetOrCreateNode(MultiTreeFloors &floors,
                                MultiTreeFloor floor,
                                MultiTreeKey key);

// Пытается перейти на следующий этаж.
// На вход получает текущий этаж и ссылку для записи следующего этажа.
// Возвращает true и записывает current + 1, либо false, если current равен 255.
bool TryGetNextFloor(MultiTreeFloor current,
                     MultiTreeFloor &next) noexcept;

// Добавляет parent в один из двух обратных указателей child, если там есть место.
// На вход получает родительский и дочерний узлы; ничего не возвращает.
// Повторное добавление того же родителя игнорируется.
template <typename T>
void AddBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

// Удаляет parent из обратных указателей child.
// На вход получает родительский и дочерний узлы; ничего не возвращает.
template <typename T>
void RemoveBackPointer(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

// Устанавливает child левым потомком parent и синхронизирует обратные указатели.
// На вход получает родительский и дочерний узлы; ничего не возвращает.
// Если parent равен nullptr, операция не выполняется.
template <typename T>
void LinkLeft(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

// Устанавливает child правым потомком parent и синхронизирует обратные указатели.
// На вход получает родительский и дочерний узлы; ничего не возвращает.
// Если parent равен nullptr, операция не выполняется.
template <typename T>
void LinkRight(MultiTreeNode<T> *parent, MultiTreeNode<T> *child);

// Получает или создает узел и устанавливает его левым потомком parent.
// На вход получает parent, таблицу этажей, этаж и ключ дочернего узла.
// Возвращает указатель на подключенный узел.
MultiTreeNode8 *LinkLeftByKey(MultiTreeNode8 *parent,
                              MultiTreeFloors &floors,
                              MultiTreeFloor childFloor,
                              MultiTreeKey childKey);

// Получает или создает узел и устанавливает его правым потомком parent.
// На вход получает parent, таблицу этажей, этаж и ключ дочернего узла.
// Возвращает указатель на подключенный узел.
MultiTreeNode8 *LinkRightByKey(MultiTreeNode8 *parent,
                               MultiTreeFloors &floors,
                               MultiTreeFloor childFloor,
                               MultiTreeKey childKey);

// Удаляет узел из таблицы его этажа, не освобождая память узла.
// На вход получает таблицу этажей и указатель на узел; ничего не возвращает.
// Для nullptr и узла с некорректным этажом операция не выполняется.
void RemoveNodeFromFloors(MultiTreeFloors &floors,
                          MultiTreeNode8 *node) noexcept;

namespace multi_tree_detail
{
    // Рекурсивно выводит дерево в консоль с обозначением ребер и общих узлов.
    // Получает текущий узел, префикс форматирования, подпись ребра,
    // признак последнего элемента и множество уже посещенных узлов.
    template <typename T>
    void PrintTreeImpl(const MultiTreeNode<T> *node,
                      const std::string &prefix,
                      const std::string &edge,
                      bool isLast,
                      std::unordered_set<const MultiTreeNode<T> *> &visited);

    // Удаляет лист и рекурсивно удаляет его родителей, ставших листьями.
    // Получает удаляемый узел и ссылку на корень; обновляет связи и может изменить root.
    template <typename T>
    void DeleteEmptyNode(MultiTreeNode<T> *node, MultiTreeNode<T> *&root);

    // Записывает узлы и ребра дерева в поток в формате Graphviz DOT.
    // Получает текущий узел, поток вывода и множество уже посещенных узлов.
    template <typename T>
    void WriteDot(const MultiTreeNode<T> *node,
                  std::ofstream &out,
                  std::unordered_set<const MultiTreeNode<T> *> &visited);

    // Вариант DeleteEmptyNode, который также удаляет узлы из таблицы этажей.
    // Получает лист, ссылку на корень и таблицу этажей; ничего не возвращает.
    void DeleteEmptyNodeWithFloors(MultiTreeNode8 *node,
                                   MultiTreeNode8 *&root,
                                   MultiTreeFloors &floors);
}

template <typename T>
// Печатает дерево в консоль в виде текстовой иерархии.
// Получает корень (или текущий узел), начальный префикс и признак последнего узла.
// Ничего не возвращает; повторно встреченные общие узлы помечаются как shared.
void PrintTree(const MultiTreeNode<T> *node,
              const std::string &prefix = "",
              bool isLast = true);

template <typename T>
// Печатает дерево в консоль в более компактном декоративном формате.
// Получает корень, начальный префикс и признак последнего узла; ничего не возвращает.
void printTreePretty(const MultiTreeNode<T> *root,
                    const std::string &prefix = "",
                    bool isLast = true);

template <typename T>
// Печатает дерево в консоль с обозначением левого ребра как 0, правого как 1.
// Получает корень, префикс форматирования и признак правого потомка; ничего не возвращает.
void printTreeLR(const MultiTreeNode<T> *root,
                const std::string &prefix = "",
                bool isRight = false);

template <typename T>
// Записывает дерево в файл в формате Graphviz DOT.
// Получает корень, открываемый поток и имя файла (по умолчанию tree.dot).
// Открывает и закрывает поток самостоятельно; ничего не возвращает.
void printTreeToFileDot(const MultiTreeNode<T> *root,
                        std::ofstream &out,
                        const std::string &fileName = "tree.dot");

template <typename T>
// Удаляет указанный лист и рекурсивно удаляет пустые узлы выше него.
// Получает лист и ссылку на корень; обновляет связи и может установить root в nullptr.
// Если узел не является листом или один из указателей равен nullptr, ничего не делает.
void DeleteFromLeafToUp(MultiTreeNode<T> *leaf,
                       MultiTreeNode<T> *&root);

// Версия DeleteFromLeafToUp для дерева с таблицей этажей.
// Дополнительно удаляет освобожденные узлы из floors; ничего не возвращает.
void DeleteFromLeafToUp(MultiTreeNode8 *leaf,
                        MultiTreeNode8 *&root,
                        MultiTreeFloors &floors);

template <typename T>
// Рекурсивно освобождает все узлы дерева, не удаляя один узел более одного раза.
// Получает узел и множество посещенных узлов; ничего не возвращает.
void DestroyMultiTree(MultiTreeNode<T> *node,
                      std::unordered_set<MultiTreeNode<T> *> &visited);

template <typename T>
// Освобождает все узлы дерева, автоматически создавая множество посещенных узлов.
// Получает корень дерева; ничего не возвращает.
void DestroyMultiTree(MultiTreeNode<T> *node) noexcept;
