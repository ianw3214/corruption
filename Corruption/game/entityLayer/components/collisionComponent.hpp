#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

// Assumes all collisions are squares
// Can consider other shapes in the future
class CollisionComponent : public Component
{
public:
    REGISTER_COMPONENT(CollisionComponent);

    CollisionComponent();
    CollisionComponent(int width, int height, int offsetX = 0, int offsetY = 0, bool passable = false);

    virtual void Read(std::ifstream& file) override;
    virtual void Write(std::ofstream& file) override;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetOffsetX() const { return m_offsetX; }
    int GetOffsetY() const { return m_offsetY; }
    bool Passable() const { return m_passable; }

    virtual void Update(float delta) override;
private:
    int m_width;
    int m_height;
    int m_offsetX;
    int m_offsetY;

    // This flag determines if the collision blocks other collisions
    bool m_passable;
    friend class EditorLayer;
};