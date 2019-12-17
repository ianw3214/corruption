#include "UILayer.hpp"

#include "oasis.h"
#include <SDL2/SDL.h>

void UILayer::Init() 
{
    SDL_ShowCursor(SDL_FALSE);
    sprite = Oasis::Sprite("res/cursor.png");
}

void UILayer::Close() 
{

}

bool UILayer::HandleEvent(const Oasis::Event& event) 
{
    if (event.GetType() == Oasis::EventType::MOUSE_MOVE)
    {
        const Oasis::MouseMovedEvent& mouseEvent = dynamic_cast<const Oasis::MouseMovedEvent&>(event);
        sprite.SetPos(mouseEvent.GetX(), Oasis::WindowService::WindowHeight() - mouseEvent.GetY());
    }
    return false;
}

void UILayer::Update()  
{
    Oasis::Renderer::DrawSprite(sprite);
}