#include "entity.hpp"

Oasis::Reference<Component> Entity::AddComponent(Component * component)
{
    component->SetEntity(this);
    m_components.push_back(component);
    return m_components.back();
}

void Entity::Update(float delta)
{
    for (Oasis::Reference<Component> component : m_components)
    {
        component->Update(delta);
    }
}