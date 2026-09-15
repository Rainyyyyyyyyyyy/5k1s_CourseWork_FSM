#include "FSM.h"

#include <limits>
#include <stdexcept>
#include <utility>
#include <iostream>

FSM::FSM(std::size_t m,
         std::vector<StateNumber> g0, std::vector<StateNumber> g1,
         std::vector<Bit> f0, std::vector<Bit> f1,
         StateNumber initialState)
    : m_(m),
      stateCount_(calculateStateCount(m)),
      g0_(std::move(g0)),
      g1_(std::move(g1)),
      f0_(std::move(f0)),
      f1_(std::move(f1)),
      initialState_(initialState),
      currentState_(initialState)
{
    if (g0_.size() != stateCount_ || g1_.size() != stateCount_ ||
        f0_.size() != stateCount_ || f1_.size() != stateCount_)
    {
        throw std::invalid_argument(
            "FSM: g0, g1, f0 and f1 must contain exactly 2^(2^M) elements");
    }
    if (initialState_ >= stateCount_)
    {
        throw std::invalid_argument("FSM: initial state is out of range");
    }

    validateTransitionTable(g0_);
    validateTransitionTable(g1_);
}

FSM::StepResult FSM::Step(Bit input)
{
    const StateNumber previous = currentState_;

    // input == false эквивалентен 0, input == true — 1.
    if (input)
    {
        currentState_ = g1_[previous];
        return StepResult{previous, currentState_, f1_[previous]};
    }

    currentState_ = g0_[previous];
    return StepResult{previous, currentState_, f0_[previous]};
}

std::vector<FSM::Bit> FSM::ProcessWord(const std::vector<Bit> &input)
{
    std::vector<Bit> output;
    output.reserve(input.size());
    for (Bit bit : input)
    {
        output.push_back(Step(bit).output);
    }
    return output;
}

void FSM::reset() noexcept
{
    currentState_ = initialState_;
}

void FSM::output() noexcept
{
    std::cout << '\t';
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t'<<i;
    }
    std::cout << '\n';

    std::cout<<"0\t";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << g0_[i];
    }
    std::cout << '\n';

    std::cout<<"1\t";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << g1_[i];
    }
    std::cout << '\n';
    ///
    ///
    ///
    std::cout<<"0\t";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << f0_[i];
    }
    std::cout << '\n';

    std::cout<<"1\t";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << f1_[i];
    }
    std::cout << '\n';

}

std::size_t FSM::GetM() const noexcept
{
    return m_;
}

FSM::StateNumber FSM::GetStateCount() const noexcept
{
    return stateCount_;
}

FSM::StateNumber FSM::GetCurrentState() const noexcept
{
    return currentState_;
}

FSM::StateNumber FSM::GetInitialState() const noexcept
{
    return initialState_;
}

const std::vector<FSM::StateNumber> &FSM::Getg0() const noexcept
{
    return g0_;
}

const std::vector<FSM::StateNumber> &FSM::Getg1() const noexcept
{
    return g1_;
}

const std::vector<FSM::Bit> &FSM::Getf0() const noexcept
{
    return f0_;
}

const std::vector<FSM::Bit> &FSM::Getf1() const noexcept
{
    return f1_;
}

FSM::StateNumber FSM::calculateStateCount(std::size_t m)
{
    constexpr std::size_t sizeBits = std::numeric_limits<StateNumber>::digits;
    if (m >= sizeBits)
    {
        throw std::invalid_argument("FSM: M is too large to calculate 2^M");
    }

    const std::size_t codeBits = std::size_t{1} << m;
    if (codeBits >= sizeBits)
    {
        throw std::invalid_argument(
            "FSM: 2^(2^M) does not fit in a state number");
    }
    return StateNumber{1} << codeBits;
}

void FSM::validateTransitionTable(const std::vector<StateNumber> &table) const
{
    for (StateNumber nextState : table)
    {
        if (nextState >= stateCount_)
        {
            throw std::invalid_argument("FSM: transition table has invalid state number");
        }
    }
}
