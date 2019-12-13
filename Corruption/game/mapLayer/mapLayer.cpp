#include "mapLayer.hpp"

#include <string>
#include <fstream>
#include <cstring>

#include "game/entityLayer/camera.hpp"

#include "util/timer.hpp"

static Shader * mapShader = nullptr;

MapSector::MapSector(int x, int y)
    : m_x(x)
    , m_y(y)
    , m_va(nullptr)
    , m_vb(nullptr)
    , m_ib(nullptr)
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
    CreateBufferData();
}

void MapSector::PutTile(int x, int y, int tile)
{
    // The x and y are raw pixel positions
    int tile_x = (x - kSectorPixelWidth * m_x) / static_cast<int>(kTileSize);
    int tile_y = (y - kSectorPixelHeight * m_y) / static_cast<int>(kTileSize);
    m_tiles[tile_y * kSectorWidth + tile_x] = tile;
    CreateBufferData();
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

void MapSector::Render(Oasis::Reference<Oasis::Sprite> sprite)
{
    int pixel_x = m_x * kSectorPixelWidth;
    int pixel_y = m_y * kSectorPixelHeight;

    Oasis::Reference<Oasis::Texture> texture = Oasis::ResourceManager::GetResource<Oasis::Texture>(sprite->GetTexturePath());

    texture->bind();
    mapShader->bind();
    mapShader->setUniform1f("u_screenWidth", static_cast<float>(Oasis::WindowService::WindowWidth()));
    mapShader->setUniform1f("u_screenHeight", static_cast<float>(Oasis::WindowService::WindowHeight()));
    mapShader->setUniform1f("u_textureWidth", static_cast<float>(texture->getWidth()));
	mapShader->setUniform1f("u_textureHeight", static_cast<float>(texture->getHeight()));
    mapShader->setUniform1f("u_cameraX", static_cast<float>(Camera::GetX()));
	mapShader->setUniform1f("u_cameraY", static_cast<float>(Camera::GetY()));
    m_va->bind();
    m_ib->bind();

    glDrawElements(GL_TRIANGLES, m_ib->getCount(), GL_UNSIGNED_INT, nullptr);    
}

void MapSector::CreateBufferData()
{
    // Clear previous buffer data if exists
    if (m_ib) delete m_ib;
    if (m_vb) delete m_vb;
    if (m_va) delete m_va;

    constexpr int num_vertices = kSectorWidth * kSectorHeight * 4;
    // Initialize raw buffer data
    float positions[num_vertices * 4];
    unsigned int indices[kSectorWidth * kSectorHeight * 6];
    // Populate buffer data
    int cur_index = 0;
    int cur_tile = 0;
    int cur_index_index = 0;
    for (int y = 0; y < kSectorHeight; ++y)
    {
        for (int x = 0; x < kSectorWidth; ++x)
        {
            OASIS_TRAP(cur_index < num_vertices * 4);
            int tile = m_tiles[y * kSectorWidth + x];
            float tile_x = static_cast<float>(tile % kTilesheetWidth) * kTileSourceSize;
            float tile_y = static_cast<float>(tile / kTilesheetWidth) * kTileSourceSize;
            positions[cur_index++] = m_x * kSectorPixelWidth + x * kTileSize;
            positions[cur_index++] = m_y * kSectorPixelHeight + y * kTileSize;
            positions[cur_index++] = tile_x;
            positions[cur_index++] = tile_y + kTileSourceSize;
            positions[cur_index++] = m_x * kSectorPixelWidth + x * kTileSize;
            positions[cur_index++] = m_y * kSectorPixelHeight + (y + 1) * kTileSize;
            positions[cur_index++] = tile_x;
            positions[cur_index++] = tile_y;
            positions[cur_index++] = m_x * kSectorPixelWidth + (x + 1) * kTileSize;
            positions[cur_index++] = m_y * kSectorPixelHeight + (y + 1) * kTileSize;
            positions[cur_index++] = tile_x + kTileSourceSize;
            positions[cur_index++] = tile_y;
            positions[cur_index++] = m_x * kSectorPixelWidth + (x + 1) * kTileSize;
            positions[cur_index++] = m_y * kSectorPixelHeight + y * kTileSize;
            positions[cur_index++] = tile_x + kTileSourceSize;
            positions[cur_index++] = tile_y + kTileSourceSize;
            OASIS_TRAP(cur_index_index < kSectorWidth * kSectorHeight * 6);
            indices[cur_index_index++] = cur_tile;
            indices[cur_index_index++] = cur_tile + 1;
            indices[cur_index_index++] = cur_tile + 2;
            indices[cur_index_index++] = cur_tile;
            indices[cur_index_index++] = cur_tile + 2;
            indices[cur_index_index++] = cur_tile + 3;
            cur_tile += 4;
        }
    }
    m_va = new VertexArray();   
    m_vb = new VertexBuffer(positions, sizeof(float) * num_vertices * 4);
    m_ib = new IndexBuffer(indices, kSectorWidth * kSectorHeight * 6);
    // specify the layout of the buffer data
    VertexBufferLayout layout;
    layout.pushFloat(2);
    layout.pushFloat(2);
    m_va->addBuffer(*m_vb, layout);
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
            m_dirtyFlags.emplace_back(false);
        }
    }
    if (!mapShader)
    {
        mapShader = new Shader("res/shaders/camera_vertex.glsl", "res/shaders/sprite_fragment.glsl");
        mapShader->setUniform1f("u_screenWidth", static_cast<float>(Oasis::WindowService::WindowWidth()));
        mapShader->setUniform1f("u_screenHeight", static_cast<float>(Oasis::WindowService::WindowHeight()));
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


void MapLayer::PutTile(int mouse_x, int mouse_y, int tile)
{
    int x = mouse_x + static_cast<int>(Camera::GetX());
    int y = mouse_y + static_cast<int>(Camera::GetY());
    // Figure out the sector from the raw x/y position
    int sector_x = x / kSectorPixelWidth;
    int sector_y = y / kSectorPixelHeight;
    if (sector_x >= 0 && sector_x < kMapWidth && sector_y >= 0 && sector_y < kMapHeight)
    {
        GetSectorAt(sector_x, sector_y).PutTile(x, y, tile);
        m_dirtyFlags[sector_y * kMapWidth + sector_x] = true;
    }
}

void MapLayer::SaveMap()
{
    OASIS_TRAP(m_sectors.size() == m_dirtyFlags.size());
    for (unsigned int i = 0; i < m_dirtyFlags.size(); ++i)
    {
        if (m_dirtyFlags[i])
        {
            m_sectors[i].SaveToFile();
            m_dirtyFlags[i] = false;
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
