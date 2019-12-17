#include "collisionComponent.hpp"
IMPL_COMPONENT(CollisionComponent);

CollisionComponent::CollisionComponent()
    : m_width(0)
    , m_height(0)
    , m_offsetX(0)
    , m_offsetY(0)
    , m_passable(false)
{
    
}

CollisionComponent::CollisionComponent(int width, int height, int offsetX, int offsetY, bool passable)
    : m_width(width)
    , m_height(height)
    , m_offsetX(offsetX)
    , m_offsetY(offsetY)
    , m_passable(passable)
{
    
}

void CollisionComponent::Read(std::ifstream& file)
{
    file.read((char*) &m_width, sizeof(int));
    file.read((char*) &m_height, sizeof(int));
    file.read((char*) &m_offsetX, sizeof(int));
    file.read((char*) &m_offsetY, sizeof(int));
}

void CollisionComponent::Write(std::ofstream& file)
{
    file.write((const char*) &m_width, sizeof(int));
    file.write((const char*) &m_height, sizeof(int));
    file.write((const char*) &m_offsetX, sizeof(int));
    file.write((const char*) &m_offsetY, sizeof(int));
}


void CollisionComponent::Update(float delta) 
{
    // Does nothing in update
}