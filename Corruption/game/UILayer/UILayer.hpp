#pragma once
#include "oasis.h"

////////////////////////////////////////////////////////////////////////////////////////
class UILayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    Oasis::Sprite m_cursorSprite;
    Oasis::Sprite m_targetSprite;
};