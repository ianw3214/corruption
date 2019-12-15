#include "editorLayer.hpp"

#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

#include <SDL2/SDL.h>
#include <imgui.h>

#include "game/game.hpp"
#include "game/mapLayer/mapLayer.hpp"
#include "game/entityLayer/camera.hpp"
#include "game/entityLayer/entity.hpp"
#include "game/entityLayer/entityLayer.hpp"

#include "game/entityLayer/components/renderComponent.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"
#include "game/entityLayer/components/healthComponent.hpp"

void EditorLayer::Init()
{
    m_editorMode = false;
    m_currTile = 0;
    m_newEntityWindow = false;

    ResetNewEntityProperties();

    m_showingFileBrowser = false;
    m_currentPath = kBaseTextureResourceDirectory;

    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
        ImGui::Begin("EDITOR", nullptr, ImGuiWindowFlags_MenuBar);
        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Save map", nullptr, nullptr))
            {
                Game::GetMapLayer()->SaveMap();
            }
            if (ImGui::MenuItem("New entity", nullptr, nullptr))
            {
                m_newEntityWindow = true;
            }
        }
        ImGui::EndMenuBar();

        ImGui::Checkbox("Editor Mode", &m_editorMode);
        ImGui::SliderInt("TILE", &m_currTile, 0, kTilesheetWidth * kTilesheetHeight);
        ImGui::End();   
    });
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        if (m_showingFileBrowser || m_newEntityWindow)
        {
            ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
            ImGui::Begin("NEW ENTITY", nullptr, ImGuiWindowFlags_MenuBar);
            if (m_showingFileBrowser)
            {
                FileBrowserWindowFunc();
            }
            else if (m_newEntityWindow)
            {
                NewEntityWindowFunc();
            }
            ImGui::End();
        }
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
    // If there's a new entity being created, render some stuff about it
    if (m_newEntityWindow)
    {
        if (m_entityRenderComp)
        {
            if (m_renderCompPath.size() > 0)
            {
                Oasis::Sprite sprite(m_renderCompPath);
                sprite.SetPos(m_newEntityX - Camera::GetX(), m_newEntityY - Camera::GetY());
                sprite.SetDimensions(m_renderCompWidth, m_renderCompHeight);
                Oasis::Renderer::DrawSprite(sprite);
            }
        }
        if (m_entityCollisionComp)
        {
            float x = m_newEntityX - Camera::GetX();
            float y = m_newEntityY - Camera::GetY();
            float w = static_cast<float>(m_collisionCompWidth);
            float h = static_cast<float>(m_collisionCompHeight);
            Oasis::Renderer::DrawLine(x, y, x, y + h, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x, y, x + w, y, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x + w, y, x + w, y + h, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x, y + h, x + w, y + h, Oasis::Colour{1.f, .5f, 0.f});
        }
    }
}

void EditorLayer::NewEntityWindowFunc()
{   
    // Get state for the new entity
    ImGui::SliderFloat("x", &m_newEntityX, 0.f, 15000.f);
    ImGui::SliderFloat("y", &m_newEntityY, 0.f, 15000.f);
    
    ImGui::Checkbox("Render component", &m_entityRenderComp);
    if (m_entityRenderComp)
    {
        ImGui::Text("texture: %s", m_renderCompPath.c_str());
        ImGui::SliderFloat("render width", &m_renderCompWidth, 0.f, 500.f);
        ImGui::SliderFloat("render height", &m_renderCompHeight, 0.f, 500.f);
        if (ImGui::Button("Change texture"))
        {
            m_showingFileBrowser = true;
        }
    }
    ImGui::Checkbox("Collision Component", &m_entityCollisionComp);
    if (m_entityCollisionComp)
    {
        ImGui::SliderInt("collision width", &m_collisionCompWidth, 0, 500);
        ImGui::SliderInt("collision height", &m_collisionCompHeight, 0, 500);
    }
    ImGui::Checkbox("Health Component", &m_entityHealthComp);
    if (m_entityHealthComp)
    {
        ImGui::SliderInt("health", &m_healthCompHealth, 0, 20);
    }

    // Export entity and close window when done
    if (ImGui::Button("Done"))
    {
        AddNewEntityToGame();
        m_newEntityWindow = false;
    }
}

void EditorLayer::FileBrowserWindowFunc()
{
    // If we are deeper into the resource folder, allow going back
    if (m_currentPath != kBaseTextureResourceDirectory)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.6f, .5f, .7f, 1.f));
        if (ImGui::Button("back"))
        {
            fs::path path = m_currentPath;
            fs::path parent = path.parent_path();
            m_currentPath = parent.string();
        }
        ImGui::PopStyleColor();
    }
    for (const auto& entry : fs::directory_iterator(m_currentPath))
    {
        fs::path path = entry.path();
        std::string str = path.string();
        if (fs::is_directory(path))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.1f, .1f, .4f, 1.f));
        }
        if (ImGui::Button(str.c_str()))
        {
            if (fs::is_directory(path))
            {
                m_currentPath = str;
            }
            else
            {
                m_renderCompPath = str;
                m_showingFileBrowser = false;
                m_currentPath = kBaseTextureResourceDirectory;
                break;
            }
        }
        if (fs::is_directory(path))
        {
            ImGui::PopStyleColor();
        }
    }
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.6f, .5f, .7f, 1.f));
    if (ImGui::Button("Cancel"))
    {
        m_showingFileBrowser = false;
        m_currentPath = kBaseTextureResourceDirectory;
    }
    ImGui::PopStyleColor();
}

void EditorLayer::AddNewEntityToGame()
{
    Entity * entity = new Entity();
    entity->SetX(m_newEntityX);
    entity->SetY(m_newEntityY);
    entity->SetSerializedX(m_newEntityX);
    entity->SetSerializedY(m_newEntityY);
    if (m_entityRenderComp)
    {
        RenderComponent * renderComp = new RenderComponent(m_renderCompPath);
        renderComp->SetDimensions(m_renderCompWidth, m_renderCompHeight);
        entity->AddComponent(renderComp);
    }
    if (m_entityCollisionComp)
    {
        CollisionComponent * collisionComp = new CollisionComponent(m_collisionCompWidth, m_collisionCompHeight);
        entity->AddComponent(collisionComp);
    }
    if (m_entityHealthComp)
    {
        HealthComponent * healthComp = new HealthComponent(m_healthCompHealth);
        entity->AddComponent(healthComp);
    }

    entity->MarkSerialized();
    Game::GetEntityLayer()->AddEntity(entity, true);

    void ResetNewEntityProperties();
}

void EditorLayer::ResetNewEntityProperties()
{
    m_newEntityX = 0.f;
    m_newEntityY = 0.f;
    m_entityRenderComp = false;
    m_renderCompWidth = 0.f;
    m_renderCompHeight = 0.f;
    m_renderCompPath = "";
    m_entityCollisionComp = false;
    m_collisionCompWidth = 0;
    m_collisionCompHeight = 0;
    m_entityHealthComp = false;
    m_healthCompHealth = 0;
}