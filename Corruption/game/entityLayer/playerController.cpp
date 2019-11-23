#pragma once
#include "playerController.hpp"

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "components/renderComponent.hpp"

Oasis::Reference<Entity> PlayerController::s_player;
bool PlayerController::m_upPressed = false;
bool PlayerController::m_downPressed = false;
bool PlayerController::m_leftPressed = false;
bool PlayerController::m_rightPressed = false;

void PlayerController::SetPlayer(Oasis::Reference<Entity> entity)
{
    s_player = entity;
    Oasis::ImGuiWrapper::AddWindowFunction(&DEBUG);
}

bool PlayerController::OnEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        const Oasis::KeyPressedEvent& keyEvent = dynamic_cast<const Oasis::KeyPressedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_W)
        {
            m_upPressed = true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            m_downPressed = true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            m_leftPressed = true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            m_rightPressed = true;
        }
    }
    if (event.GetType() == Oasis::EventType::KEY_RELEASED)
    {
        const Oasis::KeyReleasedEvent& keyEvent = dynamic_cast<const Oasis::KeyReleasedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_W)
        {
            m_upPressed = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            m_downPressed = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            m_leftPressed = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            m_rightPressed = false;
        }
    }
    return false;
}



void PlayerController::Update()
{
    OASIS_TRAP(s_player);
    float x = s_player->GetX();
    float y = s_player->GetY();

    Oasis::Reference<RenderComponent> renderComp = s_player->GetComponent<RenderComponent>();
    OASIS_TRAP(renderComp);
    auto anim = renderComp->GetAnimatedSprite();

    if (m_upPressed) 
    {
        s_player->SetY(y + 1.f);
        anim->PlayAnimation("up");
    }
    if (m_downPressed) 
    {
        s_player->SetY(y - 1.f);
        anim->PlayAnimation("down");
    }
    if (m_leftPressed) 
    {
        s_player->SetX(x - 1.f);
        anim->PlayAnimation("left");
    }
    if (m_rightPressed)
    {
        s_player->SetX(x + 1.f);
        anim->PlayAnimation("right");
    }
}

void PlayerController::DEBUG()
{

}