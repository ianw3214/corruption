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
    m_inEditor = false;
    m_movingEntity = false;
    m_editorMode = EditorMode::TILE;
    m_currTile = 0;
    m_newEntityWindow = false;

    ResetNewEntityProperties();

    m_showingFileBrowser = false;
    m_currentPath = kBaseTextureResourceDirectory;
    m_filePathTarget = nullptr;

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

        ImGui::Checkbox("Use Editor", &m_inEditor);
        const char * editorModeText = "";
        if (m_editorMode == EditorMode::TILE) editorModeText = "Editor Mode (tile)";
        if (m_editorMode == EditorMode::ENTITY) editorModeText = "Editor Mode (entity)";
        if (ImGui::TreeNode(editorModeText))
        {
            if (ImGui::Selectable("Tile mode", m_editorMode == EditorMode::TILE))
            {
                m_editorMode = EditorMode::TILE;
            }
            if (ImGui::Selectable("Entity mode", m_editorMode == EditorMode::ENTITY))
            {
                m_editorMode = EditorMode::ENTITY;
            }
            ImGui::TreePop();
        }
        ImGuiRenderTilemap();

        ImGui::End();   
    });
    // Window for creating a new entity
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        if (m_newEntityWindow)
        {
            ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
            ImGui::Begin("NEW ENTITY", nullptr, ImGuiWindowFlags_MenuBar);
            NewEntityWindowFunc();
            ImGui::End();
        }
    });
    // Show entity info if we are in entity mode
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        if (m_inEditor && m_editorMode == EditorMode::ENTITY)
        {
            EntityInfoWindowFunc();
        }
    });
    // File browser
    Oasis::ImGuiWrapper::AddWindowFunction([=](){
        if (m_showingFileBrowser)
        {
            FileBrowserWindowFunc();
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
            m_inEditor = !m_inEditor;
            return true;
        }
        if (m_inEditor)
        {
            if (keyEvent.GetKey() == SDL_SCANCODE_1)
            {
                m_editorMode = EditorMode::TILE;
            }
            if (keyEvent.GetKey() == SDL_SCANCODE_2)
            {
                m_editorMode = EditorMode::ENTITY;
            }
            if (keyEvent.GetKey() == SDL_SCANCODE_DELETE || keyEvent.GetKey() == SDL_SCANCODE_BACKSPACE)
            {
                if (m_selectedEntity)
                {
                    Game::GetEntityLayer()->DeleteEntity(m_selectedEntity, true);
                    m_selectedEntity = nullptr;
                }
            }
        }
    }
    if (event.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        const Oasis::MousePressedEvent& mouseEvent = dynamic_cast<const Oasis::MousePressedEvent&>(event);
        if (m_inEditor)
        {
            if (m_editorMode == EditorMode::TILE)
            {
                // TODO: Should probably handle camera positions here
                Oasis::Reference<MapLayer> map = Game::GetMapLayer();
                map->PutTile(mouseEvent.GetX(), Oasis::WindowService::WindowHeight() - mouseEvent.GetY(), m_currTile);
                return true;   
            }
            if (m_editorMode == EditorMode::ENTITY)
            {
                // Get the entity at the clicked position
                for (Oasis::Reference<Entity> entity : Game::GetEntityLayer()->GetEntities())
                {
                    // Only allow serializable entities to be selected
                    if (!entity->Serialized())
                    {
                        continue;
                    }
                    if (auto render = entity->GetComponent<RenderComponent>())
                    {
                        float x = entity->GetX();
                        float y = entity->GetY();
                        float w = render->GetWidth();
                        float h = render->GetHeight();
                        float mouse_x = static_cast<float>(mouseEvent.GetX()) + Camera::GetX();
                        float mouse_y = static_cast<float>(Oasis::WindowService::WindowHeight() - mouseEvent.GetY()) + Camera::GetY();
                        if (mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h)
                        {
                            // If the selected entity was already this entity, start moving it
                            if (m_selectedEntity == entity)
                            {
                                m_movingEntity = true;
                                return true;
                            }
                            else
                            {
                                m_selectedEntity = entity;
                                return true;
                            }
                        }
                    }
                    else if (auto col = entity->GetComponent<CollisionComponent>())
                    {
                        float x = entity->GetX() + col->GetOffsetX();
                        float y = entity->GetY() + col->GetOffsetY();
                        float w = static_cast<float>(col->GetWidth());
                        float h = static_cast<float>(col->GetHeight());
                        float mouse_x = static_cast<float>(mouseEvent.GetX()) + Camera::GetX();
                        float mouse_y = static_cast<float>(Oasis::WindowService::WindowHeight() - mouseEvent.GetY()) + Camera::GetY();
                        if (mouse_x >= x && mouse_x <= x + w && mouse_y >= y && mouse_y <= y + h)
                        {
                            // If the selected entity was already this entity, start moving it
                            if (m_selectedEntity == entity)
                            {
                                m_movingEntity = true;
                                return true;
                            }
                            else
                            {
                                m_selectedEntity = entity;
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    if (event.GetType() == Oasis::EventType::MOUSE_RELEASE)
    {
        m_movingEntity = false;
    }
    if (event.GetType() == Oasis::EventType::MOUSE_MOVE)
    {
        if (m_movingEntity && m_selectedEntity)
        {
            const Oasis::MouseMovedEvent& mouseEvent = dynamic_cast<const Oasis::MouseMovedEvent&>(event);
            float x = m_selectedEntity->GetSerializedX() + static_cast<float>(mouseEvent.GetXOffset());
            float y = m_selectedEntity->GetSerializedY() - static_cast<float>(mouseEvent.GetYOffset());
            m_selectedEntity->SetSerializedX(x);
            m_selectedEntity->SetSerializedY(y);
            // Also set actual X/Y so updates in game as well
            m_selectedEntity->SetX(x);
            m_selectedEntity->SetY(y);
            // Mark the map as dirty as well
            int sector_x = static_cast<int>(x / kSectorPixelWidth);
            int sector_y = static_cast<int>(y / kSectorPixelHeight);
            Game::GetMapLayer()->MarkSectorDirty(sector_x, sector_y);
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
            float x = m_newEntityX - Camera::GetX() + static_cast<float>(m_collisionCompOffsetX);
            float y = m_newEntityY - Camera::GetY() + static_cast<float>(m_collisionCompOffsetY);
            float w = static_cast<float>(m_collisionCompWidth);
            float h = static_cast<float>(m_collisionCompHeight);
            Oasis::Renderer::DrawLine(x, y, x, y + h, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x, y, x + w, y, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x + w, y, x + w, y + h, Oasis::Colour{1.f, .5f, 0.f});
            Oasis::Renderer::DrawLine(x, y + h, x + w, y + h, Oasis::Colour{1.f, .5f, 0.f});
        }
    }
    if (m_inEditor)
    {
        if (m_selectedEntity)
        {
            float x = m_selectedEntity->GetX() - Camera::GetX();
            float y = m_selectedEntity->GetY() - Camera::GetY();
            float w = 0.f;
            float h = 0.f;
            if (auto renderComp = m_selectedEntity->GetComponent<RenderComponent>())
            {
                w = renderComp->GetWidth();
                h = renderComp->GetHeight();
            }
            else if (auto collisionComp = m_selectedEntity->GetComponent<CollisionComponent>())
            {
                w = static_cast<float>(collisionComp->GetWidth());
                h = static_cast<float>(collisionComp->GetHeight());
            }
            Oasis::Renderer::DrawLine(x, y, x, y + h, Oasis::Colour{0.f, .9f, 1.f});
            Oasis::Renderer::DrawLine(x, y, x + w, y, Oasis::Colour{0.f, .9f, 1.f});
            Oasis::Renderer::DrawLine(x + w, y, x + w, y + h, Oasis::Colour{0.f, .9f, 1.f});
            Oasis::Renderer::DrawLine(x, y + h, x + w, y + h, Oasis::Colour{0.f, .9f, 1.f});
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
            m_filePathTarget = &m_renderCompPath;
        }
    }
    ImGui::Checkbox("Collision Component", &m_entityCollisionComp);
    if (m_entityCollisionComp)
    {
        ImGui::SliderInt("collision width", &m_collisionCompWidth, 0, 500);
        ImGui::SliderInt("collision height", &m_collisionCompHeight, 0, 500);
        ImGui::SliderInt("collision offset x", &m_collisionCompOffsetX, 0, 200);
        ImGui::SliderInt("collision offset y", &m_collisionCompOffsetY, 0, 200);
        ImGui::Checkbox("collision passable", &m_collisionPassable);
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
                *m_filePathTarget = str;
                m_showingFileBrowser = false;
                m_currentPath = kBaseTextureResourceDirectory;
                m_filePathTarget = nullptr;
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
        m_filePathTarget = false;
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
        CollisionComponent * collisionComp = new CollisionComponent(
            m_collisionCompWidth, 
            m_collisionCompHeight, 
            m_collisionCompOffsetX, 
            m_collisionCompOffsetY, 
            m_collisionPassable);
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
    m_collisionCompOffsetX = 0;
    m_collisionCompOffsetY = 0;
    m_entityHealthComp = false;
    m_healthCompHealth = 0;
}

void EditorLayer::ImGuiRenderTilemap()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.f, 1.f));

    Oasis::Reference<Oasis::Texture> texture = Oasis::ResourceManager::GetResource<Oasis::Texture>("res/tiles/basic.png");
    GLuint textureId = texture->GetID();
    ImVec2 buttonSize(60, 60);
    float windowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    ImGuiStyle& style = ImGui::GetStyle();
    const int numButtons = kTilesheetWidth * kTilesheetHeight;
    for (unsigned int i = 0; i < numButtons; ++i)
    {
        ImGui::PushID(i);
        ImVec2 uv0(0.f, 0.f);
        ImVec2 uv1(1.f, 1.f);
        {   // Calculate texture positions
            static const float width = static_cast<float>(kTileSourceSize) / static_cast<float>(texture->getWidth());
            static const float height = static_cast<float>(kTileSourceSize) / static_cast<float>(texture->getHeight());
            int x = i % kTilesheetWidth;
            int y = i / kTilesheetWidth;
            uv0.x = x * width;
            uv0.y = 1.f - y * height;
            uv1.x = (x + 1) * width;
            uv1.y = 1.f - (y + 1) * height;
        }
        // Determine if tint is needed based on whether tile is selected
        ImVec4 tint(1.f, 1.f, 1.f, 1.f);
        if (i == m_currTile)
        {
            tint = ImVec4(1.f, 1.f, 1.f, .3f);
        }
        // Render the actual button
        if (ImGui::ImageButton((void*) textureId, buttonSize, uv0, uv1, 0, ImVec4(), tint))
        {
            m_currTile = i;
        }
        // Check projected position to see if we should advance to a new line
        float lastButtonX2 = ImGui::GetItemRectMax().x;
        float nextButtonX2 = lastButtonX2 + style.ItemSpacing.x + buttonSize.x;
        if (i + 1 < numButtons && nextButtonX2 < windowVisible)
        {
            ImGui::SameLine();
        }
        ImGui::PopID();
    }

    ImGui::PopStyleVar(1);
}