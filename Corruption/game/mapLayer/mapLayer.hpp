#pragma once
#include "oasis.h"

#include <vector>

constexpr float kTileSize = 100.f;

// This is the width/height of the map in sectors
constexpr int kMapWidth = 10;
constexpr int kMapHeight = 10;

// This is the width/height of the sector in tiles
constexpr int kSectorWidth = 10;
constexpr int kSectorHeight = 10;
constexpr int kSectorPixelWidth = kSectorWidth * static_cast<int>(kTileSize);
constexpr int kSectorPixelHeight = kSectorHeight * static_cast<int>(kTileSize);

////////////////////////////////////////////////////////////////////////////////////////
// Each sector is a 10 by 10 section of the map that stitch together into an open world
class MapSector
{
public:
    MapSector(int x, int y);
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

    // TODO: Each map should maybe store their own sprite
    void Render(Oasis::Reference<Oasis::Sprite> sprite);
private:
    int m_x;
    int m_y;

    int m_tiles[kSectorWidth * kSectorHeight];
    void SaveToFile();
};

////////////////////////////////////////////////////////////////////////////////////////
class MapLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;

    MapSector& GetSectorAt(int x, int y);
private:
    Oasis::Sprite m_sprite;

    // Might want to store this in a better data structure in the future
    // if access needs are INTENSE or something like that
    std::vector<MapSector> m_sectors;
};