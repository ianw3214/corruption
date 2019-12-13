#pragma once

namespace Oasis
{
    class IState;
    class StateManager
    {
    public:
        static void Init(IState * state);

        static IState * CurrentState() { return m_currentState; }
        static IState * ChangeState(IState * state);

        static void Update();

    private:
        static IState * m_currentState;
        static IState * m_nextState;
    };
}