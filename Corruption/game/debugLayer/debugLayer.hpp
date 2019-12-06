#pragma once
#include "oasis.h"

////////////////////////////////////////////////////////////////////////////////////////
class DebugLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    void ImGuiWindowFunc();

    bool m_drawPhysics;
    bool m_drawHealth;  // <= this is only temporary, should remove once proper health UI is done
};