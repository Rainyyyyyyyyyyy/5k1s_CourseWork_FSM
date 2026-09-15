#pragma once

#include <cstddef>
#include <vector>

/**
 * Полностью определённый детерминированный автомат Мили.
 *
 * Входной и выходной алфавиты: {0, 1}. Код состояния имеет 2^M бит,
 * поэтому число состояний N = 2^(2^M); они нумеруются от 0 до N - 1.
 *
 * Для каждого состояния q заданы четыре значения:
 *   g0[q], f0[q] — следующее состояние и выход при входе 0;
 *   g1[q], f1[q] — следующее состояние и выход при входе 1.
 *
 * То есть g0[q] = delta(q, 0), f0[q] = lambda(q, 0),
 *          g1[q] = delta(q, 1), f1[q] = lambda(q, 1).
 */
class FSM {
public:
    using Bit = bool;
    using StateNumber = std::size_t;

    struct StepResult {
        StateNumber previousState;
        StateNumber currentState;
        Bit output;
    };

    /**
     * Создаёт автомат из полных таблиц функций.
     * Каждый из четырёх векторов обязан содержать N = 2^(2^M) элементов.
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

    /** Обрабатывает входное слово и возвращает выходное слово той же длины. */
    std::vector<Bit> ProcessWord(const std::vector<Bit>& input);

    void reset() noexcept;

     std::size_t GetM() const noexcept;
     StateNumber GetStateCount() const noexcept;
     StateNumber GetCurrentState() const noexcept;
     StateNumber GetInitialState() const noexcept;
     const std::vector<StateNumber>& Getg0() const noexcept;
     const std::vector<StateNumber>& Getg1() const noexcept;
     const std::vector<Bit>& Getf0() const noexcept;
     const std::vector<Bit>& Getf1() const noexcept;

private:
    static StateNumber calculateStateCount(std::size_t m);
    void validateTransitionTable(const std::vector<StateNumber>& table) const;

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
Пример для M = 1: N = 2^(2^1) = 4 состояния, пронумерованные 0..3.

std::vector<FSM::StateNumber> g0{0, 2, 1, 3}; // Переходы при входе 0.
std::vector<FSM::StateNumber> g1{1, 3, 0, 2}; // Переходы при входе 1.
std::vector<FSM::Bit> f0{0, 0, 1, 1};         // Выходы при входе 0.
std::vector<FSM::Bit> f1{1, 1, 0, 0};         // Выходы при входе 1.
FSM fsm(1, g0, g1, f0, f1);

fsm.process(1); // Из q=0: output=f1[0]=1, новое q=g1[0]=1.
*/
