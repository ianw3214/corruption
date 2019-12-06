#include "collisionComponent.hpp"

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