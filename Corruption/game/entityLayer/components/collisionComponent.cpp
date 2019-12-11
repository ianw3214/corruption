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

void CollisionComponent::Read(std::ifstream& file)
{
    file.read((char*) &m_width, sizeof(int));
    file.read((char*) &m_height, sizeof(int));
}

void CollisionComponent::Write(std::ofstream& file)
{
    file.write((const char*) &m_width, sizeof(int));
    file.write((const char*) &m_height, sizeof(int));
}


void CollisionComponent::Update(float delta) 
{
    // Does nothing in update
}