#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

// Assumes all collisions are squares
// Can consider other shapes in the future
class CollisionComponent : public Component
{
public:
    CollisionComponent(int width, int height, bool passable = false);

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    bool Passable() const { return m_passable; }

    virtual void Update(float delta) override;
private:
    int m_width;
    int m_height;

    // This flag determines if the collision blocks other collisions
    bool m_passable;
};