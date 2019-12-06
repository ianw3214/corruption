#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

constexpr float kDefaultSpeed = 500.f;
class ProjectileComponent : public Component
{
public:
    ProjectileComponent(float angle, float speed = kDefaultSpeed);

    virtual void Update(float delta) override;
private:
    float m_angle;
    float m_speed;
};