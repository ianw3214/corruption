#include "UILayer.hpp"

#include "oasis.h"
#include <SDL2/SDL.h>

#include "game/game.hpp"

void UILayer::Init() 
{
    SDL_ShowCursor(SDL_FALSE);
    m_cursorSprite = Oasis::Sprite("res/cursor.png");
    m_targetSprite = Oasis::Sprite("res/cursor_target.png");
}

void UILayer::Close() 
{

}

bool UILayer::HandleEvent(const Oasis::Event& event) 
{
    if (event.GetType() == Oasis::EventType::MOUSE_MOVE)
    {
        const Oasis::MouseMovedEvent& mouseEvent = dynamic_cast<const Oasis::MouseMovedEvent&>(event);
        float mouse_x = static_cast<float>(mouseEvent.GetX());
        float mouse_y = static_cast<float>(Oasis::WindowService::WindowHeight() - mouseEvent.GetY());
        m_cursorSprite.SetPos(mouse_x, mouse_y - m_cursorSprite.GetHeight());
        m_targetSprite.SetPos(mouse_x + m_targetSprite.GetWidth() / 2.f, mouse_y + m_targetSprite.GetHeight() / 2.f);
    }
    return false;
}

void UILayer::Update()  
{
    if (Game::GetEditorLayer()->Active())
    {
        Oasis::Renderer::DrawSprite(m_cursorSprite);
    }
    else
    {
        Oasis::Renderer::DrawSprite(m_targetSprite);
    }
}
