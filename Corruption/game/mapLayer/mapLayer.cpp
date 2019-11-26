#include "mapLayer.hpp"

#include "game/entityLayer/camera.hpp"

void MapLayer::Init() 
{
    m_sprite = Oasis::Sprite("res/tiles/basic.png");
    m_sprite.SetSourceDimensions(32, 32);
    m_sprite.SetSourcePos(0., 0.);
    m_sprite.SetDimensions(kTileSize, kTileSize);
    for (int i = 0; i < kMapWidth * kMapHeight; ++i)
    {
        m_map[i] = 0;
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
    for (int y = 0; y < kMapHeight; ++y)
    {
        for (int x = 0; x < kMapWidth; ++x)
        {
            m_sprite.SetPos(x * kTileSize - Camera::GetX(), y * kTileSize - Camera::GetY());
            Oasis::Renderer::DrawSprite(m_sprite);
        }
    }
}
