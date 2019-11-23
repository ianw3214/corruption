#include "entityLayer.hpp"

#include "playerController.hpp"
#include "components/renderComponent.hpp"

void EntityLayer::Init() 
{
    RenderComponent * renderComp = new RenderComponent("res/player.png", SpriteType::ANIMATED);
    renderComp->SetSourceDimensions(32, 32);
    renderComp->SetSourcePos(0, 0);

    Oasis::Reference<Entity> entity = AddPlayer(new Entity());
    entity->AddComponent(renderComp);

    entity->SetX(50.f);
    entity->SetY(50.f);
}

void EntityLayer::Close() 
{

}


bool EntityLayer::HandleEvent(const Oasis::Event& event)
{
    return PlayerController::OnEvent(event);
}

void EntityLayer::Update() 
{
    PlayerController::Update();
    for (Oasis::Reference<Entity> entity : m_entities)
    {
        entity->Update();
    }
}

Oasis::Reference<Entity> EntityLayer::AddPlayer(Entity * entity)
{
    m_player = AddEntity(entity);
    PlayerController::SetPlayer(m_player);
    return m_player;
}

Oasis::Reference<Entity> EntityLayer::AddEntity(Entity * entity)
{
    m_entities.emplace_back(entity);
    return m_entities.back();
}