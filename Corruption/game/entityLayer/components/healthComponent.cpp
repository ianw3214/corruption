#include "healthComponent.hpp"
IMPL_COMPONENT(HealthComponent);

HealthComponent::HealthComponent(int health)
    : m_health(health)
{

}

void HealthComponent::Read(std::ifstream& file) 
{
    file.read((char*) &m_health, sizeof(int));
}
void HealthComponent::Write(std::ofstream& file) 
{
    file.write((const char*) &m_health, sizeof(int));
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
