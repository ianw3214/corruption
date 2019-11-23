#pragma once
#include "oasis.h"

#include <vector>

//////////////////////////////////////////////
class Entity;
class Component
{
public:
    virtual ~Component() {}
    inline void SetEntity(Entity * entity) { m_entity = entity; }

    virtual void Update() {}
protected:
    Oasis::Reference<Entity> m_entity;
};

//////////////////////////////////////////////
class Entity
{
public:
    inline void SetX(float x) { m_x = x;}
    inline void SetY(float y) { m_y = y;}
    inline float GetX() const { return m_x; }
    inline float GetY() const { return m_y; }

    Oasis::Reference<Component> AddComponent(Component * component);

    void Update();
private:
    std::vector<Oasis::Owned<Component>> m_components;

    // Let all entities have a position
    float m_x;
    float m_y;
};