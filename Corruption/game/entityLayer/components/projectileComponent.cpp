#include "projectileComponent.hpp"
IMPL_COMPONENT(ProjectileComponent);

#include "game/game.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"

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
    auto layer = Game::GetEntityLayer();
    for (Oasis::Reference<Entity> entity : layer->GetEntities())
    {
        if (entity != m_entity)
        {
            if (Oasis::Reference<CollisionComponent> col = entity->GetComponent<CollisionComponent>())
            {
                float pX = m_entity->GetX();
                float pY = m_entity->GetY();
                int pW = pCollision->GetWidth();
                int pH = pCollision->GetHeight();
                float x = entity->GetX();
                float y = entity->GetY();
                int w = col->GetWidth();
                int h = col->GetHeight();
                if (pX < x + w && pX + pW > x)
                {
                    if (pY < y + h && pY + pH > y)
                    {
                        m_entity->Remove();
                        auto health = entity->GetComponent<HealthComponent>();
                        if (health)
                        {
                            health->TakeDamage(1);
                        }
                    }
                }
            }
        }
    }
}