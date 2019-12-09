#pragma once
#include "oasis.h"

#include <fstream>
#include <vector>


//////////////////////////////////////////////
// Used by serialization, DO NOT CHANGE EVER
#define REGISTER_COMPONENT(COMPONENT) static int s_serializerIndex;\
    virtual int GetIndex() const override { return s_serializerIndex; }\
    virtual Component * Clone() override { return new COMPONENT(); }
#define IMPL_COMPONENT(COMPONENT) int COMPONENT::s_serializerIndex;
class Entity;
class Component
{
public:
    virtual ~Component() {}
    inline void SetEntity(Entity * entity) { m_entity = entity; }
    inline Oasis::Reference<Entity> GetEntity() const { return m_entity; }
    virtual int GetIndex() const = 0;

    virtual void Update(float delta) {}

    // TODO: There has got to be a better way lmao
    virtual void Read(std::ifstream& file) {}
    virtual void Write(std::ofstream& file) {}
    virtual Component * Clone() { return nullptr; }
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
    inline bool ShouldRemove() const { return m_remove; }

    inline void Remove() { m_remove = true; }

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
    friend class EntitySerializer;
    std::vector<Oasis::Owned<Component>> m_components;

    // Let all entities have a position
    float m_x;
    float m_y;

    // Entity management
    bool m_remove = false;
};