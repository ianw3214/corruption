#pragma once
#include "oasis.h"

////////////////////////////////////////////////////////////////////////////////////////
class EditorLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    void ImGuiWindowFunc();

    bool m_editorMode;

    /////////////////////////////////////
    // EDITOR STATE
    /////////////////////////////////////
    // The tile that the 'brush' is currently using
    int m_currTile;
};