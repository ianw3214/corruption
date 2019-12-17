#include "debugLayer.hpp"

#include "game/game.hpp"
#include "game/entityLayer/entityLayer.hpp"
#include "game/entityLayer/camera.hpp"

#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"

#include "imgui.h"
void DebugLayer::Init() 
{
    m_drawPhysics = false;
    m_drawHealth = false;
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
        static bool show = true;
        ImGui::Begin("DEBUG", &show, ImGuiWindowFlags_MenuBar);

        // General debug info
        auto player = Game::GetEntityLayer()->GetPlayer();
        ImGui::Text("Player Position: (%.2f, %.2f)", player->GetX(), player->GetY());

        // Draw flags
        ImGui::Checkbox("Draw collisions", &m_drawPhysics);
        ImGui::Checkbox("Draw health", &m_drawHealth);
        ImGui::End();   
    });
}

void DebugLayer::Close() 
{

}

bool DebugLayer::HandleEvent(const Oasis::Event& event) 
{
    return false;
}

void DebugLayer::Update()  
{
    for (Oasis::Reference<Entity> entity : Game::GetEntityLayer()->GetEntities())
    {
        if (m_drawPhysics)
        {
            if (Oasis::Reference<CollisionComponent> col = entity->GetComponent<CollisionComponent>())
            {
                float x = entity->GetX() - Camera::GetX() + col->GetOffsetX();
                float y = entity->GetY() - Camera::GetY() + col->GetOffsetY();
                float w = static_cast<float>(col->GetWidth());
                float h = static_cast<float>(col->GetHeight());
                Oasis::Renderer::DrawLine(x, y, x, y + h, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x, y, x + w, y, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x + w, y, x + w, y + h, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x, y + h, x + w, y + h, Oasis::Colours::WHITE);
            }
        }
        if (m_drawHealth)
        {
            if (Oasis::Reference<HealthComponent> health = entity->GetComponent<HealthComponent>())
            {
                float x = entity->GetX() - Camera::GetX();
                float y = entity->GetY() - Camera::GetY();
                Oasis::TextRenderer::DrawString(std::to_string(health->GetHealth()), x, y, Oasis::Colours::RED);
            }
        }
    }
}
