#include "debugLayer.hpp"

#include "game/game.hpp"
#include "game/entityLayer/entityLayer.hpp"
#include "game/entityLayer/camera.hpp"

#include "game/entityLayer/components/collisionComponent.hpp"

#include "imgui.h"
void DebugLayer::Init() 
{
    m_drawPhysics = false;
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
        static bool show = true;
        ImGui::Begin("DEBUG", &show, ImGuiWindowFlags_MenuBar);
        ImGui::Checkbox("Draw collisions", &m_drawPhysics);
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
    if (m_drawPhysics)
    {
        Oasis::Reference<EntityLayer> layers = Game::GetEntityLayer();
        for (Oasis::Reference<Entity> entity : layers->GetEntities())
        {
            if (Oasis::Reference<CollisionComponent> col = entity->GetComponent<CollisionComponent>())
            {
                float x = entity->GetX() - Camera::GetX();
                float y = entity->GetY() - Camera::GetY();
                float w = static_cast<float>(col->GetWidth());
                float h = static_cast<float>(col->GetHeight());
                Oasis::Renderer::DrawLine(x, y, x, y + h, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x, y, x + w, y, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x + w, y, x + w, y + h, Oasis::Colours::WHITE);
                Oasis::Renderer::DrawLine(x, y + h, x + w, y + h, Oasis::Colours::WHITE);
            }
        }
    }
}
