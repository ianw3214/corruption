#include "entityLayer.hpp"

#include <chrono>

#include "playerController.hpp"
#include "camera.hpp"

#include "components/renderComponent.hpp"

#include "util/timer.hpp"

void EntityLayer::Init() 
{
    RenderComponent * renderComp = new RenderComponent("res/player.png", SpriteType::ANIMATED);
    renderComp->SetDimensions(120, 120);
    renderComp->SetSourceDimensions(32, 32);
    renderComp->SetSourcePos(0, 0);

    auto anim = renderComp->GetAnimatedSprite();
    anim->AddAnimation("up", 0, 0);
    anim->AddAnimation("down", 4, 4);
    anim->AddAnimation("left", 8, 8);
    anim->AddAnimation("right", 12, 12);
    anim->AddAnimation("run_up", 16, 17);
    anim->AddAnimation("run_down", 20, 21);
    anim->AddAnimation("run_left", 24, 25);
    anim->AddAnimation("run_right", 28, 29);

    Oasis::Reference<Entity> entity = AddPlayer(new Entity());
    entity->AddComponent(renderComp);

    entity->SetX(50.f);
    entity->SetY(50.f);

    // TEMPORARY CODE
    Profiler::Init();
}

void EntityLayer::Close() 
{

}


bool EntityLayer::HandleEvent(const Oasis::Event& event)
{
    return PlayerController::OnEvent(event);
}

static std::chrono::time_point<std::chrono::system_clock> lastTime;
void EntityLayer::Update() 
{
    Timer("EntityLayer::Update");

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