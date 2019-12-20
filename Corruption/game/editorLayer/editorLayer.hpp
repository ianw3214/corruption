#pragma once
#include "oasis.h"

constexpr char * kBaseTextureResourceDirectory = "res";

class Entity;

////////////////////////////////////////////////////////////////////////////////////////
enum class EditorMode
{
    TILE,
    ENTITY,
};

class EditorLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    void NewEntityWindowFunc();
    void FileBrowserWindowFunc();
    void AddNewEntityToGame();
    void ResetNewEntityProperties();

    bool m_inEditor;

    /////////////////////////////////////
    // EDITOR STATE
    /////////////////////////////////////
    EditorMode m_editorMode;
    bool m_movingEntity;
    // The entity selected when in editor mode
    Oasis::Reference<Entity> m_selectedEntity;
    // The tile that the 'brush' is currently using
    int m_currTile;
    // Show the window to create a new entity
    bool m_newEntityWindow;
    float m_newEntityX;
    float m_newEntityY;
    // Keep track of which components to put on new entity
    bool m_entityRenderComp;
    float m_renderCompWidth;
    float m_renderCompHeight;
    bool m_entityCollisionComp;
    int m_collisionCompWidth;
    int m_collisionCompHeight;
    int m_collisionCompOffsetX;
    int m_collisionCompOffsetY;
    bool m_collisionPassable;
    bool m_entityHealthComp;
    int m_healthCompHealth;
    std::string m_renderCompPath;

    /////////////////////////////////////
    // FILE BROWSER
    /////////////////////////////////////
    bool m_showingFileBrowser;
    std::string m_currentPath;

    //////////////////////////////////////
    // IMGUi HELPER FUNCTIONS
    //////////////////////////////////////
    void ImGuiRenderTilemap();
};