#include "entityLayer.hpp"

#include "components/renderComponent.hpp"

void EntityLayer::Init() 
{
    Oasis::Reference<Entity> entity = AddEntity(new Entity());
    entity->AddComponent(new RenderComponent("res/player.png"));

    entity->SetX(50.f);
    entity->SetY(50.f);
}

void EntityLayer::Close() 
{

}


bool EntityLayer::HandleEvent(const Oasis::Event& event)
{
    return false;
}

void EntityLayer::Update() 
{
    for (Oasis::Reference<Entity> entity : m_entities)
    {
        entity->Update();
    }
}

Oasis::Reference<Entity> EntityLayer::AddEntity(Entity * entity)
{
    m_entities.emplace_back(entity);
    return m_entities.back();
}