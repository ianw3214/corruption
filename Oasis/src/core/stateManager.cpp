#include "stateManager.hpp"
#include "state.hpp"
using namespace Oasis;

#include "util/trap.hpp"

IState * StateManager::m_currentState;
IState * StateManager::m_nextState;

void StateManager::Init(IState * state)
{
    OASIS_TRAP(state);
    m_currentState = state;
    m_nextState = nullptr;
    state->Init();
}

IState * StateManager::ChangeState(IState * state)
{
    OASIS_TRAP(state);
    m_nextState = state;
    return state;
}

void StateManager::Update()
{
    if (m_nextState)
    {
        m_currentState->Close();
        delete m_currentState;
        m_currentState = m_nextState;
        m_nextState = nullptr;
        m_currentState->Init();
    }
}