#pragma once
#include "playerController.hpp"

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "entityLayer.hpp"
#include "camera.hpp"

#include "game/entityLayer/components/renderComponent.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/projectileComponent.hpp"

#include "game/entityLayer/systems/collision.hpp"

# define M_PI           3.14159265358979323846  /* pi */

Oasis::Reference<Entity> PlayerController::s_player;
Oasis::Reference<EntityLayer> PlayerController::s_game;
bool PlayerController::s_upHeld = false;
bool PlayerController::s_downHeld = false;
bool PlayerController::s_leftHeld = false;
bool PlayerController::s_rightHeld = false;
PlayerController::Direction PlayerController::s_direction = Direction::RIGHT;
float PlayerController::s_attackCooldown = 0.f;

void PlayerController::SetPlayer(Oasis::Reference<Entity> entity)
{
    s_player = entity;
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
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            s_downHeld = true;
            s_direction = Direction::DOWN;
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            s_leftHeld = true;
            s_direction = Direction::LEFT;
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            s_rightHeld = true;
            s_direction = Direction::RIGHT;
            return true;
        }
    }
    if (event.GetType() == Oasis::EventType::KEY_RELEASED)
    {
        const Oasis::KeyReleasedEvent& keyEvent = dynamic_cast<const Oasis::KeyReleasedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_W)
        {
            s_upHeld = false;
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_S)
        {
            s_downHeld = false;
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_A)
        {
            s_leftHeld = false;
            return true;
        }
        if (keyEvent.GetKey() == SDL_SCANCODE_D)
        {
            s_rightHeld = false;
            return true;
        }
    }
    
    // PLAYER ATTACK HANDLING
    if (event.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        if (s_attackCooldown <= 0.f)
        {
            const Oasis::MousePressedEvent& mouseEvent = dynamic_cast<const Oasis::MousePressedEvent&>(event);
            // Calculate the angle to shoot the projectile at
            float x_diff = s_player->GetX() - static_cast<float>(Camera::GetX() + mouseEvent.GetX());
            float y_diff = s_player->GetY() - static_cast<float>(Camera::GetY() + mouseEvent.GetY());
            float angle = std::atan2(y_diff, -x_diff);
            // Create a projectile entity on attack
            {   // TEMPORARY ENTITY TESTING CODE
                RenderComponent * renderComp = new RenderComponent("res/attack.png");
                renderComp->SetDimensions(60, 60);
                CollisionComponent * collisionComp = new CollisionComponent(60, 60, true);
                ProjectileComponent * projectileComp = new ProjectileComponent(angle);
                Oasis::Reference<Entity> entity = s_game->AddEntity(new Entity());
                entity->AddComponent(renderComp);
                entity->AddComponent(collisionComp);
                entity->AddComponent(projectileComp);
                
                // This is super hard coded to not collide with the player
                entity->SetX(s_player->GetX() + std::cos(angle) * 140.f + 30.f);
                entity->SetY(s_player->GetY() + std::sin(angle) * 140.f + 30.f);
            }
            // Figure out what animation to play depending on the angle
            {
                Oasis::Reference<RenderComponent> renderComp = s_player->GetComponent<RenderComponent>();
                OASIS_TRAP(renderComp);
                auto anim = renderComp->GetAnimatedSprite();
                if (angle > M_PI / 4.f && angle < M_PI * 3.f / 4.f) 
                {
                    anim->PlayAnimation("attack_up");
                    anim->QueueAnimation("up");
                }
                if (angle > -M_PI * 3.f / 4.f && angle < -M_PI / 4.f)
                {
                    anim->PlayAnimation("attack_down");
                    anim->QueueAnimation("down");
                } 
                if (angle <= -M_PI * 3.f / 4.f || angle >= M_PI * 3.f / 4.f) 
                {
                    anim->PlayAnimation("attack_left");
                    anim->QueueAnimation("left");
                }
                if (angle >= -M_PI / 4.f && angle <= M_PI / 4.f) 
                {
                    anim->PlayAnimation("attack_right");
                    anim->QueueAnimation("right");
                }
            }
            s_attackCooldown = kPlayerAttackCooldown;
        }
        return true;
    }

    return false;
}

constexpr float speed = 300.f;
void PlayerController::Update(float delta)
{
    OASIS_TRAP(s_player);
    float x = s_player->GetX();
    float y = s_player->GetY();

    // Update player movement if not attacking
    if (s_attackCooldown <= 0.f)
    {
        if (s_upHeld) 
        {
            s_player->SetY(y + speed * delta);
            while(Collision::Colliding(s_player))
            {
                s_player->SetY(std::floor(s_player->GetY() - 1));
            }
        }
        if (s_downHeld) 
        {
            s_player->SetY(y - speed * delta);
            while(Collision::Colliding(s_player))
            {
                s_player->SetY(std::ceil(s_player->GetY() + 1));
            }
        }
        if (s_leftHeld) 
        {
            s_player->SetX(x - speed * delta);
            while(Collision::Colliding(s_player))
            {
                s_player->SetX(std::ceil(s_player->GetX() + 1));
            }
        }
        if (s_rightHeld)
        {
            s_player->SetX(x + speed * delta);
            while(Collision::Colliding(s_player))
            {
                s_player->SetX(std::floor(s_player->GetX() - 1));
            }
        }
    }

    // Update cooldowns
    if (s_attackCooldown > 0.f) s_attackCooldown -= delta;

    UpdateAnimations(delta);
}

void PlayerController::UpdateAnimations(float delta)
{
    Oasis::Reference<RenderComponent> renderComp = s_player->GetComponent<RenderComponent>();
    OASIS_TRAP(renderComp);
    auto anim = renderComp->GetAnimatedSprite();

    if (s_attackCooldown <= 0.f)
    {
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
}