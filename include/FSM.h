#ifndef FSM_H
#define FSM_H

#include <cstddef>
#include <vector>
#include <bitset>

#define MAX_M_SIZE_FOR_FSM (size_t)16
#define MAX_MP_SIZE_FOR_FSM (size_t)1 << MAX_M_SIZE_FOR_FSM

/**
 * Полностью определённый детерминированный автомат Мили.
 *
 * Входной и выходной алфавиты: {0, 1}. Код состояния имеет M бит,
 * поэтому число состояний N = 2^M; они нумеруются от 0 до N - 1.
 *
 * Для каждого состояния q заданы четыре значения:
 *   g0[q], f0[q] — следующее состояние и выход при входе 0;
 *   g1[q], f1[q] — следующее состояние и выход при входе 1.
 *
 * То есть g0[q] = delta(q, 0), f0[q] = lambda(q, 0),
 *          g1[q] = delta(q, 1), f1[q] = lambda(q, 1).
 *
 * size_t M - число бит в коде состояния (всего состояний будет 2^M)
 *
 * хранения f2(0, •) и f2(1, •) выполнено с помощью std::bitset, однако из-за того, что
 * индексация в bitset идёт справа налево, то было принятно решение при выводе "разворачивать" содержимое
 * этих bitset, но в остальной работе интерпретировать бит для i-го состояния как f2(•, i)
 */
template <std::size_t M>
class FSM
{
public:
    // номер состояния (код состояния) — целое число от 0 до 2^M - 1
    using StateNumber = std::size_t;

    // вспомогательная структура для результата такта
    struct StepResult
    {
        const StateNumber previousState;
        const StateNumber currentState;
        const bool output;
    };

    /**
     * Создаёт автомат из полных таблиц функций.
     * Каждый из четырёх векторов обязан содержать M = 2^M элементов.
     * Все номера в g0 и g1 должны быть в диапазоне [0, N).
     */
    FSM(std::vector<StateNumber> g0, std::vector<StateNumber> g1,
        std::bitset<M> f0, std::bitset<M> f1,
        const StateNumber &initialState = 0,
        const StateNumber &currentState = 0)
        : g0_(g0), // g0_(std::move(g0)),
          g1_(g1), // g1_(std::move(g1)),
          f0_(f0), // f0_(std::move(f0)),
          f1_(f1), // f1_(std::move(f1)),
          initialState_(initialState),
          currentState_(initialState)
    {
        if (g0_.size() != stateCount_ || g1_.size() != stateCount_)
        {
            throw std::invalid_argument("FSM: g0, g1, f0 and f1 must contain exactly 2^M elements");
        }
        if (initialState_ >= stateCount_)
        {
            throw std::invalid_argument("FSM: initial state is out of range");
        }

        validateTransitionTable(g0_);
        validateTransitionTable(g1_);
    }

    /*
     * Конструктор по строке (из-за особенности std::bitset -
     * в результате f2(1) и f2(0) будут записаны в обратном порядке)
     */
    FSM(std::vector<StateNumber> g0, std::vector<StateNumber> g1,
        std::string f0, std::string f1,
        const StateNumber &initialState = 0,
        const StateNumber &currentState = 0)
        : g0_(g0), // g0_(std::move(g0)),
          g1_(g1), // g1_(std::move(g1)),
          // f0_(f0),
          // f1_(f1),
          initialState_(initialState),
          currentState_(initialState)
    {
        if (g0_.size() != stateCount_ || g1_.size() != stateCount_ ||
            f0.size() != stateCount_ || f1.size() != stateCount_)
        {
            throw std::invalid_argument("FSM: g0, g1, f0 and f1 must contain exactly 2^M elements");
        }
        if (initialState_ >= stateCount_)
        {
            throw std::invalid_argument("FSM: initial state is out of range");
        }
        std::reverse(f0.begin(), f0.end());
        std::reverse(f1.begin(), f1.end());
        f0_ = std::bitset<M>(f0);
        f1_ = std::bitset<M>(f1);

        validateTransitionTable(g0_);
        validateTransitionTable(g1_);
    }

    // конструктор копирования = delete
    FSM(const FSM &) = delete;

    // конструктор перемещения = delete
    FSM(FSM &&) = delete;

    /**
     * Выполняет такт для входного бита input.
     * При input == 0 используются g0 и f0, при input == 1 : g1 и f1.
     * меняет currentState_.
     */
    StepResult Step(const bool &input);

    // не выводит результат, только меняет состояние currentState_
    void StepFast(const bool &input)
    {
        const StateNumber previous = currentState_;

        // input == false эквивалентен 0, input == true — 1.
        currentState_ = input ? g1_[previous] : g0_[previous];
    }

    /** Обрабатывает входное слово и возвращает выходное слово той же длины. */
    template <size_t N>
    std::bitset<M * N> ProcessWord(const std::bitset<N> &input); //(const std::vector<Bit> &input);

    /**
     * Возвращает последовательность z(t) целиком.
     */
    template <size_t N>
    std::bitset<M * N> MakeAllPeriodReturnZ(const std::bitset<N> &u); //(std::vector<FSM::Bit> u);

    /*   * ставит генератор в начальное состояние;
     * Вырабатывает z(t) и возвращает траекторию состояний
     */
    template <size_t N>
    std::vector<typename FSM<M>::StateNumber> MakeAllPeriodReturnY(const std::bitset<N> &u); //(std::vector<FSM::Bit> u);

    void reset() noexcept;
    void output() const noexcept;

    inline const StateNumber &GetCurrentState() const noexcept { return currentState_; }
    inline const StateNumber &GetInitialState() const noexcept { return initialState_; }
    inline const std::vector<StateNumber> &Getg0() const noexcept { return g0_; }
    inline const std::vector<StateNumber> &Getg1() const noexcept { return g1_; }
    inline const std::bitset<M> &Getf0() const noexcept { return f0_; }; // inline const std::bitset<MAX_MP_SIZE_FOR_FSM> /*std::vector<Bit>*/ &Getf0() const noexcept { return f0_; }
    inline const std::bitset<M> &Getf1() const noexcept { return f1_; }; // inline const std::bitset<MAX_MP_SIZE_FOR_FSM> /*std::vector<Bit>*/ &Getf1() const noexcept { return f1_; }

    inline const bool Get_bit_from_state_by_input(const StateNumber &state, const bool &input) const noexcept;
    inline const StateNumber Get_nextState_from_state_by_input(const StateNumber &state, const bool &input) const noexcept;
    inline const bool Get_bit_from_currentState_by_input(const bool &input) const noexcept;
    inline const StateNumber Get_nextState_from_currentState_by_input(const bool &input) const noexcept;

private:
    static inline StateNumber getMP();

    // проверка на то, что номера в table находятся в пределах [0, N), где N = 2^M
    // * но не проверяет на то, что это биекция (подстановка), то есть на повторение элементов
    void validateTransitionTable(const std::vector<StateNumber> &table) const;

    // std::size_t m_;
    // StateNumber stateCount_;
    std::vector<StateNumber> g0_;
    std::vector<StateNumber> g1_;
    std::bitset<M> f0_; // std::bitset<MAX_MP_SIZE_FOR_FSM> f0_;   //std::vector<Bit> f0_;
    std::bitset<M> f1_; // std::bitset<MAX_MP_SIZE_FOR_FSM> f1_;   //std::vector<Bit> f1_;
    const StateNumber initialState_;
    StateNumber currentState_;
};

/*
Пример для M = 2: N = 2^2 = 4 состояния, пронумерованные 0..3.

std::vector<FSM::StateNumber> g0{0, 2, 1, 3}; // Переходы при входе 0.
std::vector<FSM::StateNumber> g1{1, 3, 0, 2}; // Переходы при входе 1.
std::vector<FSM::Bit> f0{0, 0, 1, 1};         // Выходы при входе 0.
std::vector<FSM::Bit> f1{1, 1, 0, 0};         // Выходы при входе 1.
FSM fsm(2, g0, g1, f0, f1);

fsm.process(1); // Из q=0: output=f1[0]=1, новое q=g1[0]=1.
*/

/**
 * Выполняет такт для входного бита input.
 * При input == 0 используются g0 и f0, при input == 1 : g1 и f1.
 * меняет currentState_.
 */
template <size_t M>
typename FSM<M>::StepResult FSM<M>::Step(const bool &input)
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

template <size_t M>
void FSM<M>::reset() noexcept
{
    currentState_ = initialState_;
}

template <size_t M>
template <size_t N>
std::bitset<M * N> FSM<M>::ProcessWord(const std::bitset<N> &input) //(const std::vector<Bit> &input)
{
    std::vector<Bit> output;
    output.reserve(input.size());
    for (Bit bit : input)
    {
        output.push_back(Step(bit).output);
    }
    return output;
}

template <size_t M>
void FSM<M>::output() const noexcept
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
    for (std::size_t i = stateCount_ - 1; i >= 0; --i)
    {
        std::cout << '\t' << f0_[i];
    }
    std::cout << '\n';

    std::cout << "1";
    for (std::size_t i = stateCount_ - 1; i >= 0; --i)
    {
        std::cout << '\t' << f1_[i];
    }
    std::cout << '\n';
    std::cout << "Current state: " << currentState_ << '\n';
}

template <size_t M>
typename FSM<M>::StateNumber FSM<M>::getMP()
{
    constexpr std::size_t sizeBits = std::numeric_limits<StateNumber>::digits;
    if (M >= sizeBits || M > MAX_M_SIZE_FOR_FSM)
    {
        throw std::invalid_argument("FSM: M is too large to calculate 2^M");
    }

    return static_cast<StateNumber>(1) << M;
}

template <size_t M>
void FSM<M>::validateTransitionTable(const std::vector<StateNumber> &table) const
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
 * соответствующе std::bitset записывает српва налево (поэтому при выводе разорачиваем)
 */
template <size_t M>
template <size_t N>
std::bitset<M * N> FSM<M>::MakeAllPeriodReturnZ(const std::bitset<N> &u) //(std::vector<FSM::Bit> u)
{
    size_t n_mask = N - 1; // 2^n - 1 = 001111 (n единиц) //u.size() - 1;
    size_t m_mask = ((static_cast<size_t>(1)) << M) - 1;
    std::bitset<M * N> z; // std::vector<FSM::Bit> z;
    size_t MN = (static_cast<size_t>(1)) << (N + M);
    z.reserve(MN);

    reset();

    for (size_t i = 0; i < MN; i++)
    {
        z.set(i); // z.push_back(Get_bit_from_currentState_by_input(u[i & n_mask]));
        StepFast(u[i & n_mask]);
    }

    return z;
}

/*   * ставит генератор в начальное состояние;
 * Вырабатывает z(t) и возвращает траекторию состояний y(t)
 */
template <size_t M>
template <size_t N>
std::vector<typename FSM<M>::StateNumber> FSM<M>::MakeAllPeriodReturnY(const std::bitset<N> &u)
{
    size_t n_mask = N - 1; // u.size() - 1;
    std::vector<typename FSM<M>::StateNumber> Y;
    size_t MN = (static_cast<size_t>(1)) << (N + M);
    Y.reserve(MN); // резервируем память, но size всё ещё 0

    reset();

    for (size_t i = 0; i < MN; i++)
    {
        // Y.push_back(Get_nextState_from_currentState_by_input(u[i & n_mask]));
        StepFast(u[i & n_mask]);
        Y.push_back(currentState_);
    }

    return Y;
}

// геттер : f2(input, state)
template <size_t M>
const bool FSM<M>::Get_bit_from_state_by_input(const StateNumber &state, const bool &input) const noexcept
{
    return input ? f1_[state] : f0_[state];
}

// геттер : g2(input, state)
template <size_t M>
const typename FSM<M>::StateNumber FSM<M>::Get_nextState_from_state_by_input(const StateNumber &state, const bool &input) const noexcept
{
    return input ? g1_[state] : g0_[state];
}

// геттер : f2(input, current_State)
template <size_t M>
const bool FSM<M>::Get_bit_from_currentState_by_input(const bool &input) const noexcept
{
    return input ? f1_[currentState_] : f0_[currentState_];
}

// геттер : g2(input, current_State)
template <size_t M>
inline const typename FSM<M>::StateNumber FSM<M>::Get_nextState_from_currentState_by_input(const bool &input) const noexcept
{
    return input ? g1_[currentState_] : g0_[currentState_];
}

#endif // FSM_H