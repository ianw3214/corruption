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
    inline Oasis::Reference<Entity> GetEntity() const { return m_entity; }

    virtual void Update(float delta) {}
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
    template<typename COMPONENT>
    Oasis::Reference<COMPONENT> GetComponent()
    {
        for (Oasis::Reference<Component> component : m_components)
        {
            if (Oasis::Reference<COMPONENT> res = Oasis::DynamicCast<COMPONENT>(component))
            {
                return res;
            }
        }
        return Oasis::Reference<COMPONENT>();
    }

    void Update(float delta);
private:
    std::vector<Oasis::Owned<Component>> m_components;

    // Let all entities have a position
    float m_x;
    float m_y;
};