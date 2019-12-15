#pragma once
#include "oasis.h"

#include <vector>

constexpr float kTileSize = 100.f;

// This is the width/height of the map in sectors
constexpr int kMapWidth = 10;
constexpr int kMapHeight = 10;

// This is the width/height of the sector in tiles
constexpr int kSectorWidth = 15;
constexpr int kSectorHeight = 15;
constexpr int kSectorPixelWidth = kSectorWidth * static_cast<int>(kTileSize);
constexpr int kSectorPixelHeight = kSectorHeight * static_cast<int>(kTileSize);

// Hard coded tile metadata - will need to load from file if using more than 1 tilemap in the future
constexpr float kTileSourceSize = 32.f;
constexpr int kTilesheetWidth = 8;
constexpr int kTilesheetHeight = 8;

////////////////////////////////////////////////////////////////////////////////////////
// Each sector is a 10 by 10 section of the map that stitch together into an open world
class MapSector
{
public:
    MapSector(int x, int y);
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

    void PutTile(int x, int y, int tile);
    void SaveToFile();

    void Render(Oasis::Reference<Oasis::Sprite> sprite);
private:
    int m_x;
    int m_y;

    int m_tiles[kSectorWidth * kSectorHeight];

    // Store buffers to avoid having to draw it every time
    VertexArray * m_va;
    VertexBuffer * m_vb;
    IndexBuffer * m_ib;
    void CreateBufferData();
};

////////////////////////////////////////////////////////////////////////////////////////
class MapLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;

    void LoadSectors();

    void PutTile(int mouse_x, int mouse_y, int tile);
    void SaveMap();
    void MarkSectorDirty(int sector_x, int sector_y);

    MapSector& GetSectorAt(int x, int y);
private:
    Oasis::Sprite m_sprite;

    // Might want to store this in a better data structure in the future
    // if access needs are INTENSE or something like that
    std::vector<MapSector> m_sectors;

    // These store temporary state that are serialization utilities
    std::vector<bool> m_dirtyFlags;
};