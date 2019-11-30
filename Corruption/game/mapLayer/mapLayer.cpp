#include "mapLayer.hpp"

#include <string>
#include <fstream>
#include <cstring>

#include "game/entityLayer/camera.hpp"

#include "util/timer.hpp"

MapSector::MapSector(int x, int y)
    : m_x(x)
    , m_y(y)
{
    std::string filename("res/maps/" + std::to_string(x) + "-" + std::to_string(y) + ".dat");
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.read((char*) m_tiles, sizeof(m_tiles));
    }
    else
    {
        // Create a new file if it doesn't exist
        std::memset(m_tiles, 0, sizeof(m_tiles));
        SaveToFile();
    }
}

void MapSector::Render(Oasis::Reference<Oasis::Sprite> sprite)
{
    int pixel_x = m_x * kSectorPixelWidth - static_cast<int>(Camera::GetX());
    int pixel_y = m_y * kSectorPixelHeight - static_cast<int>(Camera::GetY());

    if (pixel_x + kSectorPixelWidth < 0 || pixel_y + kSectorPixelHeight < 0) return;
    if (pixel_x > Oasis::WindowService::WindowWidth() || pixel_y > Oasis::WindowService::WindowHeight()) return;

    for (int y = 0; y < kSectorHeight; ++y)
    {
        for (int x = 0; x < kSectorWidth; ++x)
        {
            sprite->SetPos(pixel_x + x * kTileSize, pixel_y + y * kTileSize);
            Oasis::Renderer::DrawSprite(sprite);
        }
    }
}

void MapSector::SaveToFile()
{
    std::string filename("res/maps/" + std::to_string(m_x) + "-" + std::to_string(m_y) + ".dat");
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open())
    {
        file.write((const char*) m_tiles, sizeof(m_tiles));
    }
    else
    {
        OASIS_TRAP(false);
    }
}

void MapLayer::Init() 
{
    m_sprite = Oasis::Sprite("res/tiles/basic.png");
    m_sprite.SetSourceDimensions(32, 32);
    m_sprite.SetSourcePos(0., 0.);
    m_sprite.SetDimensions(kTileSize, kTileSize);

    // Initialize some sectors
    for (int x = 0; x < kMapWidth; ++x)
    {
        for (int y = 0; y < kMapHeight; ++y)
        {
            m_sectors.emplace_back(x, y);
        }
    }
}

void MapLayer::Close() 
{

}


bool MapLayer::HandleEvent(const Oasis::Event& event)
{
    return false;
}

void MapLayer::Update() 
{
    // TEMPORARY CODE
    Profiler::Update();
    Timer("MapLayer::Update");
    for (int y = 0; y < kMapHeight; ++y)
    {
        for (int x = 0; x < kMapWidth; ++x)
        {
            // m_sprite.SetPos(x * kTileSize - Camera::GetX(), y * kTileSize - Camera::GetY());
            // Oasis::Renderer::DrawSprite(m_sprite);
            GetSectorAt(x, y).Render(m_sprite);
        }
    }
}

MapSector& MapLayer::GetSectorAt(int x, int y)
{
    for (MapSector& sector : m_sectors)
    {
        if (sector.GetX() == x && sector.GetY() == y)
        {
            return sector;
        }
    }

    OASIS_TRAP(false);
    return m_sectors[0];
}
