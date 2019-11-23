#pragma once
#include "oasis.h"

constexpr int kMapWidth = 20;
constexpr int kMapHeight = 20;
constexpr float kTileSize = 100.f;

class MapLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
    Oasis::Sprite m_sprite;

    int m_map[kMapWidth * kMapHeight];
};