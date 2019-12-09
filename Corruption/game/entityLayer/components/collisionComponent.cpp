#include "collisionComponent.hpp"
IMPL_COMPONENT(CollisionComponent);

CollisionComponent::CollisionComponent()
    : m_width(0)
    , m_height(0)
    , m_passable(false)
{
    
}

CollisionComponent::CollisionComponent(int width, int height, bool passable)
    : m_width(width)
    , m_height(height)
    , m_passable(passable)
{
    
}

void CollisionComponent::Update(float delta) 
{
    // Does nothing in update
}