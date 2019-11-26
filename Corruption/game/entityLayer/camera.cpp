#include "camera.hpp"
#include "entity.hpp"

Oasis::Reference<Entity> Camera::s_player;
float Camera::s_cameraX;
float Camera::s_cameraY;

void Camera::SetPlayer(Oasis::Reference<Entity> entity)
{
    s_player = entity;
}

float Camera::GetX()
{
    return s_cameraX;
}

float Camera::GetY()
{
    return s_cameraY;
}

void Camera::Update(float delta)
{
    OASIS_TRAP(s_player.GetData());
    s_cameraX = s_player->GetX() - static_cast<float>(Oasis::WindowService::WindowWidth()) / 2.f;
    s_cameraY = s_player->GetY() - static_cast<float>(Oasis::WindowService::WindowHeight()) / 2.f;
}