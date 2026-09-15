#include "FSM.h"
#include "BinaryTree.h"

Tree Reverse_first_2powN(const FSM &fsm, std::vector<FSM::Bit> &output, const size_t &N, size_t newState, Node<FSM::StateNumber> *newNode, size_t i = 0)
{
    static const size_t pow2N = 1 << N;
    if (i < pow2N)
    {
        if (fsm.Get_bit_from_state_by_input(newState, 0) == output[i])
        {
            Node<FSM::StateNumber> *leftNode = new Node<FSM::StateNumber>;//(fsm.Getg0()[newState]);
            newNode->left = leftNode;
            newNode->state = (fsm.Getg0()[newState]);
            Reverse_first_2powN(fsm, output, N, newNode->state, leftNode, i + 1);
        }
        if (fsm.Get_bit_from_state_by_input(newState, 1) == output[i])
        {
            Node<FSM::StateNumber> *rightNode = new Node<FSM::StateNumber>;//(fsm.Getg0()[newState]);
            newNode->right = rightNode;
            newNode->state = (fsm.Getg0()[newState]);
            Reverse_first_2powN(fsm, output, N, newNode->state, rightNode, i + 1);
        }

    }
}