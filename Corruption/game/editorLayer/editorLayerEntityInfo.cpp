#include "editorLayer.hpp"

#include <imgui.h>

#include "game/game.hpp"
#include "game/mapLayer/mapLayer.hpp"
#include "game/entityLayer/entity.hpp"
#include "game/entityLayer/components/renderComponent.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"

void EditorLayer::EntityInfoWindowFunc()
{
    ImGui::Begin("Entity Info", nullptr, ImGuiWindowFlags_MenuBar);
    if (m_selectedEntity)
    {
        ImGui::Text("position: (%.2f, %.2f)", m_selectedEntity->GetSerializedX(), m_selectedEntity->GetSerializedY());
        // Render component
        if (auto render = m_selectedEntity->GetComponent<RenderComponent>())
        {   
            ImGui::Text("texture: %s", render->GetSprite()->GetTexturePath().c_str());
            float width = render->GetWidth();
            float height = render->GetHeight();
            ImGui::SliderFloat("render width", &width, 0.f, 500.f);
            ImGui::SliderFloat("render height", &height, 0.f, 500.f);
            if (ImGui::Button("Change texture"))
            {
                // TODO: Implement
            }
            render->SetDimensions(width, height);
        }
        // Collision component
        if (auto collision = m_selectedEntity->GetComponent<CollisionComponent>())
        {   
            ImGui::SliderInt("collision width", &(collision->m_width), 0, 500);
            ImGui::SliderInt("collision height", &(collision->m_height), 0, 500);
            ImGui::SliderInt("collision offset x", &(collision->m_offsetX), 0, 200);
            ImGui::SliderInt("collision offset y", &(collision->m_offsetY), 0, 200);
            ImGui::Checkbox("collision passable", &(collision->m_passable));
        }
        // Health component
        if (auto health = m_selectedEntity->GetComponent<HealthComponent>())
        {   
            ImGui::SliderInt("health", &(health->m_health), 0, 20);
        }
        // TODO: This is a super hack, fix this properly
        // Mark the map as dirty as well
        int sector_x = static_cast<int>(m_selectedEntity->GetSerializedX() / kSectorPixelWidth);
        int sector_y = static_cast<int>(m_selectedEntity->GetSerializedY() / kSectorPixelHeight);
        Game::GetMapLayer()->MarkSectorDirty(sector_x, sector_y);
    }
    else
    {
        ImGui::Text("No entity selected...");
    }
    
    ImGui::End();
}