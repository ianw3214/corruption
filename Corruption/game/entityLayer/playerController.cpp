#pragma once
#include "playerController.hpp"

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "entityLayer.hpp"

#include "components/renderComponent.hpp"
#include "components/collisionComponent.hpp"

Oasis::Reference<Entity> PlayerController::s_player;
Oasis::Reference<EntityLayer> PlayerController::s_game;
bool PlayerController::s_upHeld = false;
bool PlayerController::s_downHeld = false;
bool PlayerController::s_leftHeld = false;
bool PlayerController::s_rightHeld = false;
PlayerController::Direction PlayerController::s_direction;

void PlayerController::SetPlayer(Oasis::Reference<Entity> entity)
{
    s_player = entity;
    Oasis::ImGuiWrapper::AddWindowFunction(&DEBUG);
}

void PlayerController::SetGame(Oasis::Reference<EntityLayer> layer)
{
    s_game = layer;
}

bool PlayerController::OnEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        const Oasis::KeyPressedEvent& keyEvent = dynamic_cast<const Oasis::KeyPressedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_W)
        {
            s_upHeld = true;
            s_direction = Direction::UP;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            s_downHeld = true;
            s_direction = Direction::DOWN;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            s_leftHeld = true;
            s_direction = Direction::LEFT;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            s_rightHeld = true;
            s_direction = Direction::RIGHT;
        }
    }
    if (event.GetType() == Oasis::EventType::KEY_RELEASED)
    {
        const Oasis::KeyReleasedEvent& keyEvent = dynamic_cast<const Oasis::KeyReleasedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_W)
        {
            s_upHeld = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            s_downHeld = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            s_leftHeld = false;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            s_rightHeld = false;
        }
    }
    return false;
}

constexpr float speed = 300.f;
void PlayerController::Update(float delta)
{
    OASIS_TRAP(s_player);
    float x = s_player->GetX();
    float y = s_player->GetY();

    Oasis::Reference<RenderComponent> renderComp = s_player->GetComponent<RenderComponent>();
    OASIS_TRAP(renderComp);
    auto anim = renderComp->GetAnimatedSprite();

    if (s_upHeld) 
    {
        s_player->SetY(y + speed * delta);
        while(PlayerColliding())
        {
            s_player->SetY(s_player->GetY() - 1);
        }
    }
    if (s_downHeld) 
    {
        s_player->SetY(y - speed * delta);
        while(PlayerColliding())
        {
            s_player->SetY(s_player->GetY() + 1);
        }
    }
    if (s_leftHeld) 
    {
        s_player->SetX(x - speed * delta);
        while(PlayerColliding())
        {
            s_player->SetX(s_player->GetX() + 1);
        }
    }
    if (s_rightHeld)
    {
        s_player->SetX(x + speed * delta);
        while(PlayerColliding())
        {
            s_player->SetX(s_player->GetX() - 1);
        }
    }

    // TODO: TEMPORARY CODE, COME UP WITH BETTER FIX
    bool recalculate = false;
    if (s_direction == Direction::UP && ! s_upHeld) recalculate = true;
    if (s_direction == Direction::DOWN && ! s_downHeld) recalculate = true;
    if (s_direction == Direction::LEFT && ! s_leftHeld) recalculate = true;
    if (s_direction == Direction::RIGHT && ! s_rightHeld) recalculate = true;
    if (recalculate)
    {
        if (s_upHeld) s_direction = Direction::UP;
        if (s_downHeld) s_direction = Direction::DOWN;
        if (s_leftHeld) s_direction = Direction::LEFT;
        if (s_rightHeld) s_direction = Direction::RIGHT;
    }

    // TODO: Use a player state system for this
    if (s_upHeld || s_downHeld || s_leftHeld || s_rightHeld)
    {
        if (s_direction == Direction::UP) anim->PlayAnimation("run_up");
        if (s_direction == Direction::DOWN) anim->PlayAnimation("run_down");
        if (s_direction == Direction::LEFT) anim->PlayAnimation("run_left");
        if (s_direction == Direction::RIGHT) anim->PlayAnimation("run_right");
    }
    else
    {
        if (s_direction == Direction::UP) anim->PlayAnimation("up");
        if (s_direction == Direction::DOWN) anim->PlayAnimation("down");
        if (s_direction == Direction::LEFT) anim->PlayAnimation("left");
        if (s_direction == Direction::RIGHT) anim->PlayAnimation("right");
    }
}

void PlayerController::DEBUG()
{

}

bool PlayerController::PlayerColliding()
{
    float pX = s_player->GetX();
    float pY = s_player->GetY();
    int pWidth = s_player->GetComponent<CollisionComponent>()->GetWidth();
    int pHeight = s_player->GetComponent<CollisionComponent>()->GetHeight();

    for (Oasis::Reference<Entity> ent : s_game->GetEntities())
    {
        if (ent == s_player)
        {
            continue;
        }
        if (Oasis::Reference<CollisionComponent> col = ent->GetComponent<CollisionComponent>())
        {
            float x = ent->GetX();
            float y = ent->GetY();
            int width = col->GetWidth();
            int height = col->GetHeight();
            if (pX < x + width && pX + pWidth > x)
            {
                if (pY < y + height && pY + pHeight > y)
                {
                    return true;
                }
            }
        }
    }
    return false;
}