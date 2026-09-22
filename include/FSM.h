#pragma once

#include <cstddef>
#include <vector>

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
 */
class FSM
{
public:
    using Bit = bool;
    using StateNumber = std::size_t;

    struct StepResult
    {
        StateNumber previousState;
        StateNumber currentState;
        Bit output;
    };

    /**
     * Создаёт автомат из полных таблиц функций.
     * Каждый из четырёх векторов обязан содержать N = 2^M элементов.
     * Все номера в g0 и g1 должны быть в диапазоне [0, N).
     */
    FSM(std::size_t m,
        std::vector<StateNumber> g0, std::vector<StateNumber> g1,
        std::vector<Bit> f0, std::vector<Bit> f1,
        StateNumber initialState = 0);

    /**
     * Выполняет такт для входного бита input.
     * При input == 0 используются g0 и f0, при input == 1 — g1 и f1.
     */
    StepResult Step(Bit input);
    void StepFast(Bit input);

    /** Обрабатывает входное слово и возвращает выходное слово той же длины. */
    std::vector<Bit> ProcessWord(const std::vector<Bit> &input);

    /**
     * Возвращает последовательность z(t) целиком.
     */
    std::vector<FSM::Bit> MakeAllPeriodReturnZ(FSM &fsm, std::vector<FSM::Bit> u);

    /*   * ставит генератор в начальное состояние;
     * Вырабатывает z(t) и возвращает траекторию состояний
     */
    std::vector<FSM::StateNumber> MakeAllPeriodReturnY(FSM &fsm, std::vector<FSM::Bit> u);

    void reset() noexcept;
    void output() noexcept;

    inline std::size_t GetM() const noexcept;
    inline StateNumber GetStateCount() const noexcept;
    inline StateNumber GetCurrentState() const noexcept;
    inline StateNumber GetInitialState() const noexcept;
    inline const std::vector<StateNumber> &Getg0() const noexcept;
    inline const std::vector<StateNumber> &Getg1() const noexcept;
    inline const std::vector<Bit> &Getf0() const noexcept;
    inline const std::vector<Bit> &Getf1() const noexcept;

    inline const Bit Get_bit_from_state_by_input(StateNumber state, Bit input) const noexcept;
    inline const StateNumber Get_nextState_from_state_by_input(StateNumber state, Bit input) const noexcept;
    inline const Bit Get_bit_from_currentState_by_input(Bit input) const noexcept;
    inline const StateNumber Get_nextState_from_currentState_by_input(Bit input) const noexcept;

private:
    inline static StateNumber calculateStateCount(std::size_t m);
    void validateTransitionTable(const std::vector<StateNumber> &table) const;

    std::size_t m_;
    StateNumber stateCount_;
    std::vector<StateNumber> g0_;
    std::vector<StateNumber> g1_;
    std::vector<Bit> f0_;
    std::vector<Bit> f1_;
    StateNumber initialState_;
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
