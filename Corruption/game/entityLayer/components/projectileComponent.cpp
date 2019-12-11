#include "projectileComponent.hpp"
IMPL_COMPONENT(ProjectileComponent);

#include "game/game.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"
#include "game/entityLayer/systems/collision.hpp"

ProjectileComponent::ProjectileComponent()
    : m_angle(0.f)
    , m_speed(0.f)
{

}

ProjectileComponent::ProjectileComponent(float angle, float speed)
    : m_angle(angle)
    , m_speed(speed)
{

}

void ProjectileComponent::Update(float delta)
{
    float x_offset = std::cos(m_angle) * m_speed * delta;
    float y_offset = std::sin(m_angle) * m_speed * delta;
    m_entity->SetX(m_entity->GetX() + x_offset);
    m_entity->SetY(m_entity->GetY() + y_offset);

    Oasis::Reference<CollisionComponent> pCollision = m_entity->GetComponent<CollisionComponent>();
    if (!pCollision)
    {
        Oasis::Console::Print("PROJECTILE MISSING COLLISION COMPONENT");
        return;
    }
    // Check to see if a collision occured
    if (auto target = Collision::Colliding(m_entity))
    {
        m_entity->Remove();
        auto health = target->GetComponent<HealthComponent>();
        if (health)
        {
            health->TakeDamage(1);
        }
    }
}