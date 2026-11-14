#pragma once

#include "../FSM.h"

#include <bitset>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/**
 * Фабрика экземпляров FSM<M>.
 *
 * Параметр M известен во время компиляции, поэтому его нужно указывать
 * при вызове фабрики: FSMFactory::Create<M>(...).
 */
class FSMFactory
{
public:
    template <std::size_t M>
    static std::unique_ptr<FSM<M>> CreateFromBitsetAs_f2(
        const std::vector<typename FSM<M>::StateNumber>& g0,
        const std::vector<typename FSM<M>::StateNumber>& g1,
        const std::bitset<FSM<M>::stateCount_>& f0,
        const std::bitset<FSM<M>::stateCount_>& f1,
        typename FSM<M>::StateNumber initialState = 0,
        typename FSM<M>::StateNumber currentState = 0);

    template <std::size_t M>
    static std::unique_ptr<FSM<M>> CreateFromStringAs_f2(
        const std::vector<typename FSM<M>::StateNumber>& g0,
        const std::vector<typename FSM<M>::StateNumber>& g1,
        const std::string& f0,
        const std::string& f1,
        typename FSM<M>::StateNumber initialState = 0,
        typename FSM<M>::StateNumber currentState = 0);
};

template <std::size_t M>
std::unique_ptr<FSM<M>> FSMFactory::CreateFromBitsetAs_f2(
    const std::vector<typename FSM<M>::StateNumber>& g0,
    const std::vector<typename FSM<M>::StateNumber>& g1,
    const std::bitset<FSM<M>::stateCount_>& f0,
    const std::bitset<FSM<M>::stateCount_>& f1,
    typename FSM<M>::StateNumber initialState,
    typename FSM<M>::StateNumber currentState)
{
    return std::unique_ptr<FSM<M>>(
        new FSM<M>(g0, g1, f0, f1, initialState, currentState));
}

template <std::size_t M>
std::unique_ptr<FSM<M>> FSMFactory::CreateFromStringAs_f2(
    const std::vector<typename FSM<M>::StateNumber>& g0,
    const std::vector<typename FSM<M>::StateNumber>& g1,
    const std::string& f0,
    const std::string& f1,
    typename FSM<M>::StateNumber initialState,
    typename FSM<M>::StateNumber currentState)
{
    return std::unique_ptr<FSM<M>>(
        new FSM<M>(g0, g1, f0, f1, initialState, currentState));
}