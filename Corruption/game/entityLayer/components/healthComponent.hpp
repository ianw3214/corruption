#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

constexpr int kDefaultHealth = 5;
class HealthComponent : public Component
{
public:
    REGISTER_COMPONENT(HealthComponent);

    HealthComponent(int health = kDefaultHealth);

    virtual void Read(std::ifstream& file) override;
    virtual void Write(std::ofstream& file) override;

    inline int GetHealth() const { return m_health; }
    void TakeDamage(int damage);
    bool IsDead() const;

    // virtual void Update(float delta) override;
private:
    int m_health;
};