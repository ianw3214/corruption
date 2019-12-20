#include "entityLayer.hpp"

#include <chrono>
#include <algorithm>

#include "game/game.hpp"

#include "game/entityLayer/playerController.hpp"
#include "game/entityLayer/camera.hpp"
#include "game/entityLayer/entitySerializer.hpp"

#include "game/entityLayer/components/renderComponent.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/interactableComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"

#include "systems/collision.hpp"

#include "util/timer.hpp"

void EntityLayer::Init() 
{
    // TODO: Maybe put these in the systems folder
    InteractionManager::Init();
    Collision::Init(this);
    PlayerController::SetGame(this);
    {   // TEMPORARY PLAYER CODE
        RenderComponent * renderComp = new RenderComponent("res/player.png", SpriteType::ANIMATED);
        renderComp->SetDimensions(120, 120);
        renderComp->SetSourceDimensions(32, 32);
        renderComp->SetSourcePos(0, 0);

        CollisionComponent * collisionComp = new CollisionComponent(120, 120);

        HealthComponent * healthComp = new HealthComponent(10);

        auto anim = renderComp->GetAnimatedSprite();
        anim->AddAnimation("up", 0, 0);
        anim->AddAnimation("down", 4, 4);
        anim->AddAnimation("left", 8, 8);
        anim->AddAnimation("right", 12, 12);
        anim->AddAnimation("run_up", 16, 19);
        anim->AddAnimation("run_down", 20, 23);
        anim->AddAnimation("run_left", 24, 27);
        anim->AddAnimation("run_right", 28, 31);
        anim->AddAnimation("attack_up", 32, 34);
        anim->AddAnimation("attack_down", 36, 38);
        anim->AddAnimation("attack_left", 40, 42);
        anim->AddAnimation("attack_right", 44, 46);
        anim->SetFPS(10);

        Oasis::Reference<Entity> entity = AddPlayer(new Entity());
        entity->AddComponent(renderComp);
        entity->AddComponent(collisionComp);
        entity->AddComponent(healthComp);

        entity->SetX(7000.f);
        entity->SetY(7000.f);
    }
}

void EntityLayer::Close() 
{

}


bool EntityLayer::HandleEvent(const Oasis::Event& event)
{
    if (InteractionManager::OnEvent(event)) return true;
    return PlayerController::OnEvent(event);
}

static std::chrono::time_point<std::chrono::system_clock> lastTime;
void EntityLayer::Update() 
{
    Timer("EntityLayer::Update");

    // Sort the entities based on their y position
    // This could blow up, might need to rethink the solution as number of entities goes up
    std::sort(m_entities.begin(), m_entities.end(), 
    [](Oasis::Reference<Entity> a, Oasis::Reference<Entity> b) -> bool
    { 
        return a->GetY() > b->GetY(); 
    });

    // Delta time calculation
    auto delta = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastTime).count()) / 1000.f;
    lastTime = std::chrono::system_clock::now();
    if (delta < 0.f || delta > 500.f)
    {
        delta = 1000.f / 24.f;
    }

    // Actual updates
    PlayerController::Update(delta);
    Camera::Update(delta);
    for (Oasis::Reference<Entity> entity : m_entities)
    {
        entity->Update(delta);
    }

    // Remove entities
    auto it = m_entities.begin();
    while(it != m_entities.end())
    {
        if ((*it)->ShouldRemove())
        {
            it = m_entities.erase(it);
        }
        else
        {
            it++;
        }
        
    }
}

Oasis::Reference<Entity> EntityLayer::AddPlayer(Entity * entity)
{
    m_player = AddEntity(entity);
    PlayerController::SetPlayer(m_player);
    Camera::SetPlayer(m_player);
    return m_player;
}

Oasis::Reference<Entity> EntityLayer::AddEntity(Entity * entity, bool markMapDirty)
{
    if (markMapDirty)
    {
        // Make sure to mark the map that the entity was placed in as dirty so it can be saved
        int sector_x = static_cast<int>(entity->GetSerializedX() / kSectorPixelWidth);
        int sector_y = static_cast<int>(entity->GetSerializedY() / kSectorPixelHeight);
        Game::GetMapLayer()->MarkSectorDirty(sector_x, sector_y);
    }

    m_entities.emplace_back(entity);
    return m_entities.back();
}

bool EntityLayer::DeleteEntity(Oasis::Reference<Entity> entity, bool markMapDirty)
{
    if (markMapDirty)
    {
        // Make sure to mark the map that the entity was placed in as dirty so it can be saved
        int sector_x = static_cast<int>(entity->GetSerializedX() / kSectorPixelWidth);
        int sector_y = static_cast<int>(entity->GetSerializedY() / kSectorPixelHeight);
        Game::GetMapLayer()->MarkSectorDirty(sector_x, sector_y);
    }
    auto it = m_entities.begin();
    while (it != m_entities.end())
    {
        if (entity == (*it))
        {
            it = m_entities.erase(it);
            return true;
        }
        else
        {
            it++;
        }
    }
    return false;
}

std::vector<Oasis::Owned<Entity>>& EntityLayer::GetEntities()
{
    return m_entities;
}