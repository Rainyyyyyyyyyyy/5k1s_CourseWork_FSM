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
            "FSM: g0, g1, f0 and f1 must contain exactly 2^M elements");
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

inline std::size_t FSM::GetM() const noexcept
{
    return m_;
}

inline FSM::StateNumber FSM::GetStateCount() const noexcept
{
    return stateCount_;
}

inline FSM::StateNumber FSM::GetCurrentState() const noexcept
{
    return currentState_;
}

inline FSM::StateNumber FSM::GetInitialState() const noexcept
{
    return initialState_;
}

inline const std::vector<FSM::StateNumber> &FSM::Getg0() const noexcept
{
    return g0_;
}

inline const std::vector<FSM::StateNumber> &FSM::Getg1() const noexcept
{
    return g1_;
}

inline const std::vector<FSM::Bit> &FSM::Getf0() const noexcept
{
    return f0_;
}

inline const std::vector<FSM::Bit> &FSM::Getf1() const noexcept
{
    return f1_;
}

inline FSM::StateNumber FSM::calculateStateCount(std::size_t m)
{
    constexpr std::size_t sizeBits = std::numeric_limits<StateNumber>::digits;
    if (m >= sizeBits)
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

inline const FSM::Bit FSM::Get_bit_from_state_by_input(StateNumber state, Bit input) const noexcept
{
    return input ? f1_[state] : f0_[state];
}
inline const FSM::Bit FSM::Get_bit_from_currentState_by_input(Bit input) const noexcept
{
    return input ? f1_[currentState_] : f0_[currentState_];
}
inline const FSM::StateNumber FSM::Get_nextState_from_state_by_input(StateNumber state, Bit input) const noexcept{
    return input ? g1_[state] : g0_[state];
}
inline const FSM::StateNumber FSM::Get_nextState_from_currentState_by_input(Bit input) const noexcept{
    return input ? g1_[currentState_] : g0_[currentState_];
}

/*   * ставит генератор в начальное состояние;
 * Вырабатывает и возвращает последовательность z(t) целиком.
 */
std::vector<FSM::Bit> FSM::MakeAllPeriodReturnZ(FSM &fsm, std::vector<FSM::Bit> u)
{
    // size_t n = u.size();
    size_t n_mask = u.size() - 1;
    // size_t m = fsm.GetM();
    size_t m_mask = ((size_t)1 << fsm.GetM()) - 1;
    std::vector<FSM::Bit> z;
    size_t MN = (size_t)1 << (u.size() + fsm.GetM());
    z.reserve(MN);
    fsm.reset();

    for (size_t i = 0; i < MN; i++)
    {
        z.push_back(fsm.Get_bit_from_currentState_by_input(u[i & n_mask]));
        fsm.StepFast(u[i & n_mask]);
    }

    return z;
}


/*   * ставит генератор в начальное состояние;
 * Вырабатывает z(t) и возвращает траекторию состояний
 */
std::vector<FSM::StateNumber> FSM::MakeAllPeriodReturnY(FSM &fsm, std::vector<FSM::Bit> u)
{
    // size_t n = u.size();
    size_t n_mask = u.size() - 1;
    // size_t m = fsm.GetM();
    size_t m_mask = ((size_t)1 << fsm.GetM()) - 1;
    std::vector<FSM::StateNumber> Y;
    size_t MN = (size_t)1 << (u.size() + fsm.GetM());
    Y.reserve(MN);
    fsm.reset();

    for (size_t i = 0; i < MN; i++)
    {
        Y.push_back(fsm.Get_nextState_from_currentState_by_input(u[i & n_mask]));
        fsm.StepFast(u[i & n_mask]);
    }

    return Y;
}
