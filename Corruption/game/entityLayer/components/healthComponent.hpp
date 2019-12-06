#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

constexpr int kDefaultHealth = 5;
class HealthComponent : public Component
{
public:
    HealthComponent(int health = kDefaultHealth);

    inline int GetHealth() const { return m_health; }
    void TakeDamage(int damage);
    bool IsDead() const;

    // virtual void Update(float delta) override;
private:
    int m_health;
};