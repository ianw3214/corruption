#include "healthComponent.hpp"
IMPL_COMPONENT(HealthComponent);

HealthComponent::HealthComponent(int health)
    : m_health(health)
{

}

void HealthComponent::TakeDamage(int damage)
{
    m_health -= damage;
    if (m_health < 0) m_health = 0;
}

bool HealthComponent::IsDead() const
{
    return m_health <= 0;
}
