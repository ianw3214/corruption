#include "gameState.hpp"
using namespace Oasis;

void GameState::Init() 
{
    InitLayers();
}

void GameState::Close()
{
    for (auto& layer : m_layers)
    {
        layer->Close();
    }
}

void GameState::OnEvent(const Event& event)
{
    for (int i = static_cast<int>(m_layers.size()) - 1; i >= 0; --i)
    {
        bool handled = m_layers[i]->HandleEvent(event);
        if (handled) break;
    }
}

void GameState::Update()
{
    for(auto& layer : m_layers)
    {
        layer->Update();
    }
}


Reference<GameStateLayer> GameState::AddLayer(GameStateLayer * layer)
{
    layer->Init();
    m_layers.push_back(layer);
    return Reference<GameStateLayer>(m_layers[m_layers.size() - 1]);
}