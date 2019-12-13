#include "editorLayer.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>

#include "game/game.hpp"
#include "game/mapLayer/mapLayer.hpp"

void EditorLayer::Init()
{
    m_editorMode = false;
    m_currTile = 0;
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
        static bool show = true;
        ImGui::Begin("EDITOR", &show, ImGuiWindowFlags_MenuBar);
        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Save map", nullptr, nullptr))
            {
                Game::GetMapLayer()->SaveMap();
            }
        }
        ImGui::EndMenuBar();

        ImGui::Checkbox("Editor Mode", &m_editorMode);
        ImGui::SliderInt("TILE", &m_currTile, 0, kTilesheetWidth * kTilesheetHeight);
        ImGui::End();   
    });
}

void EditorLayer::Close()
{

}

bool EditorLayer::HandleEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::KEY_PRESSED)
    {
        const Oasis::KeyPressedEvent& keyEvent = dynamic_cast<const Oasis::KeyPressedEvent&>(event);
        if (keyEvent.GetKey() == SDL_SCANCODE_T)
        {
            m_editorMode = !m_editorMode;
            return true;
        }
    }
    if (event.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        const Oasis::MousePressedEvent& mouseEvent = dynamic_cast<const Oasis::MousePressedEvent&>(event);
        if (m_editorMode)
        {
            Oasis::Reference<MapLayer> map = Game::GetMapLayer();
            map->PutTile(mouseEvent.GetX(), Oasis::WindowService::WindowHeight() - mouseEvent.GetY(), m_currTile);
            return true;
        }
    }
    return false;
}

void EditorLayer::Update() 
{

}