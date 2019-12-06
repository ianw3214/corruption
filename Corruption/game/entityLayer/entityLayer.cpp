#include "entityLayer.hpp"

#include <chrono>
#include <algorithm>

#include "playerController.hpp"
#include "camera.hpp"

#include "components/renderComponent.hpp"
#include "components/collisionComponent.hpp"
#include "components/interactableComponent.hpp"
#include "components/healthComponent.hpp"

#include "util/timer.hpp"

void EntityLayer::Init() 
{
    InteractionManager::Init();
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
        anim->SetFPS(10);

        Oasis::Reference<Entity> entity = AddPlayer(new Entity());
        entity->AddComponent(renderComp);
        entity->AddComponent(collisionComp);
        entity->AddComponent(healthComp);

        entity->SetX(50.f);
        entity->SetY(50.f);
    }

    {   // TEMPORARY ENTITY TESTING CODE
        RenderComponent * renderComp = new RenderComponent("res/house.png");
        CollisionComponent * collisionComp = new CollisionComponent(300, 200);
        Oasis::Reference<Entity> entity = AddEntity(new Entity());
        entity->AddComponent(renderComp);
        entity->AddComponent(collisionComp);

        entity->SetX(300.f);
        entity->SetY(300.f);
    }

    {   // TEMPORARY ENTITY TESTING CODE
        RenderComponent * renderComp = new RenderComponent("res/npc.png");
        renderComp->SetDimensions(120, 120);
        CollisionComponent * collisionComp = new CollisionComponent(120, 120);
        InteractableComponent * interactComp = new InteractableComponent([](){
            Oasis::Console::Print("TEST INTERACTION");
        });
        Oasis::Reference<Entity> entity = AddEntity(new Entity());
        entity->AddComponent(renderComp);
        entity->AddComponent(collisionComp);
        entity->AddComponent(interactComp);

        entity->SetX(400.f);
        entity->SetY(100.f);
    }

    {   // TEMPORARY ENTITY TESTING CODE
        RenderComponent * renderComp = new RenderComponent("res/enemy.png");
        renderComp->SetDimensions(120, 120);
        CollisionComponent * collisionComp = new CollisionComponent(120, 120);
        HealthComponent * healthComp = new HealthComponent();
        Oasis::Reference<Entity> entity = AddEntity(new Entity());
        entity->AddComponent(renderComp);
        entity->AddComponent(collisionComp);
        entity->AddComponent(healthComp);

        entity->SetX(50.f);
        entity->SetY(400.f);
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
}

Oasis::Reference<Entity> EntityLayer::AddPlayer(Entity * entity)
{
    m_player = AddEntity(entity);
    PlayerController::SetPlayer(m_player);
    Camera::SetPlayer(m_player);
    return m_player;
}

Oasis::Reference<Entity> EntityLayer::AddEntity(Entity * entity)
{
    m_entities.emplace_back(entity);
    return m_entities.back();
}

std::vector<Oasis::Owned<Entity>>& EntityLayer::GetEntities()
{
    return m_entities;
}