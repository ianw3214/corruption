#pragma once
#include "oasis.h"

constexpr char * kBaseTextureResourceDirectory = "res/";

class Entity;

////////////////////////////////////////////////////////////////////////////////////////
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

    bool m_editorMode;

    /////////////////////////////////////
    // EDITOR STATE
    /////////////////////////////////////
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
    std::string m_renderCompPath;

    /////////////////////////////////////
    // FILE BROWSER
    /////////////////////////////////////
    bool m_showingFileBrowser;
};