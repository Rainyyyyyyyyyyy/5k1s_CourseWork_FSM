#include "FSM.h"

#include <limits>
#include <stdexcept>
#include <utility>
#include <iostream>

FSM::FSM(std::size_t m,
         std::vector<StateNumber> g0, std::vector<StateNumber> g1,
         std::bitset<MAX_MP_SIZE_FOR_FSM> f0, std::bitset<MAX_MP_SIZE_FOR_FSM> f1,
         // std::vector<Bit> f0, std::vector<Bit> f1,
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
    if (g0_.size() != stateCount_ || g1_.size() != stateCount_)
    {
        throw std::invalid_argument(
            "FSM: g0, g1, f0 and f1 must contain exactly 2^M elements");
    }
    if (initialState_ >= stateCount_)
    {
        throw std::invalid_argument("FSM: initial state is out of range");
    }

    validateTransitionTable(g0_);
    validateTransitionTable(g1_);
}

FSM::FSM(std::size_t m,
         std::vector<StateNumber> g0, std::vector<StateNumber> g1,
         std::string f0, std::string f1,
         // std::vector<Bit> f0, std::vector<Bit> f1,
         StateNumber initialState) : m_(m),
                                     stateCount_(calculateStateCount(m)),
                                     g0_(std::move(g0)),
                                     g1_(std::move(g1)),
                                     // f0_(f0),
                                     // f1_(f1),
                                     initialState_(initialState),
                                     currentState_(initialState)
{
    if (g0_.size() != stateCount_ || g1_.size() != stateCount_ ||
        f0.size() != stateCount_ || f1.size() != stateCount_)
    {
        throw std::invalid_argument(
            "FSM: g0, g1, f0 and f1 must contain exactly 2^M elements");
    }
    if (initialState_ >= stateCount_)
    {
        throw std::invalid_argument("FSM: initial state is out of range");
    }

    f0_ = std::bitset<MAX_MP_SIZE_FOR_FSM>(f0);
    f1_ = std::bitset<MAX_MP_SIZE_FOR_FSM>(f1);
    
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

void FSM::StepFast(Bit input)
{
    const StateNumber previous = currentState_;

    // input == false эквивалентен 0, input == true — 1.
    currentState_ = input ? g1_[previous] : g0_[previous];
    return;
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
    // std::cout << '\t';
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << i;
    }
    std::cout << '\n';

    std::cout << "0";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << g0_[i];
    }
    std::cout << '\n';

    std::cout << "1";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << g1_[i];
    }
    std::cout << '\n';
    ///
    ///
    ///
    std::cout << "0";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << f0_[i];
    }
    std::cout << '\n';

    std::cout << "1";
    for (std::size_t i = 0; i < stateCount_; ++i)
    {
        std::cout << '\t' << f1_[i];
    }
    std::cout << '\n';
    std::cout << "Current state: " << currentState_ << '\n';
}

FSM::StateNumber FSM::calculateStateCount(std::size_t m)
{
    constexpr std::size_t sizeBits = std::numeric_limits<StateNumber>::digits;
    if (m >= sizeBits || m > MAX_M_SIZE_FOR_FSM)
    {
        throw std::invalid_argument("FSM: M is too large to calculate 2^M");
    }

    return StateNumber{1} << m;
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

/*   * ставит генератор в начальное состояние;
 * Вырабатывает и возвращает последовательность z(t) целиком.
 */
std::vector<FSM::Bit> FSM::MakeAllPeriodReturnZ(std::vector<FSM::Bit> u)
{
    size_t n_mask = u.size() - 1;
    size_t m_mask = ((size_t)1 << GetM()) - 1;
    std::vector<FSM::Bit> z;
    size_t MN = (size_t)1 << (u.size() + GetM());
    z.reserve(MN);
    reset();

    for (size_t i = 0; i < MN; i++)
    {
        z.push_back(Get_bit_from_currentState_by_input(u[i & n_mask]));
        StepFast(u[i & n_mask]);
    }

    return z;
}

/*   * ставит генератор в начальное состояние;
 * Вырабатывает z(t) и возвращает траекторию состояний
 */
std::vector<FSM::StateNumber> FSM::MakeAllPeriodReturnY(std::vector<FSM::Bit> u)
{
    size_t n_mask = u.size() - 1;
    size_t m_mask = ((size_t)1 << GetM()) - 1;
    std::vector<FSM::StateNumber> Y;
    size_t MN = (size_t)1 << (u.size() + GetM());
    Y.reserve(MN);
    reset();

    for (size_t i = 0; i < MN; i++)
    {
        Y.push_back(Get_nextState_from_currentState_by_input(u[i & n_mask]));
        StepFast(u[i & n_mask]);
    }

    return Y;
}
