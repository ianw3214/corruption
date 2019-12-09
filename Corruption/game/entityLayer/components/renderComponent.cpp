#include "renderComponent.hpp"
IMPL_COMPONENT(RenderComponent);
using namespace Oasis;

#include "game/entityLayer/camera.hpp"

RenderComponent::RenderComponent()
    : m_sprite(nullptr)
    , m_type(SpriteType::NORMAL)
    , m_width(0)
    , m_height(0)
{
    
}

RenderComponent::RenderComponent(const std::string& path, SpriteType type)
    : m_type(type)
    , m_width(0)
    , m_height(0)
{
    if (type == SpriteType::NORMAL)
    {
        m_sprite = new Sprite(path);
        m_width = m_sprite->GetWidth();
        m_height = m_sprite->GetHeight();
    }
    if (type == SpriteType::ANIMATED)
    {
        Reference<Texture> texture = ResourceManager::GetResource<Texture>(path);
        OASIS_TRAP(texture.GetData());
        m_sprite = new AnimatedSprite(
            path, 
            static_cast<float>(texture->getWidth()), 
            static_cast<float>(texture->getHeight())
        );
    }
}

void RenderComponent::Read(std::ifstream& file) 
{
    int size;
    file.read((char*) &size, sizeof(int));
    char * buffer = new char[size + 1];
    buffer[size] = 0;
    file.read(buffer, size);
    int type;
    file.read((char*) &type, sizeof(int));

    // Read other important state
    file.read((char*) &m_width, sizeof(float));
    file.read((char*) &m_height, sizeof(float));

    // INITIALIZATION CODE
    // TODO: CAN SHARE WITH CONSTRUCTOR PROBABLY
    std::string path(buffer);
    m_type = static_cast<SpriteType>(type);
    if (m_type == SpriteType::NORMAL)
    {
        m_sprite = new Sprite(path);
    }
    if (m_type == SpriteType::ANIMATED)
    {
        Reference<Texture> texture = ResourceManager::GetResource<Texture>(path);
        OASIS_TRAP(texture.GetData());
        m_sprite = new AnimatedSprite(
            path, 
            static_cast<float>(texture->getWidth()), 
            static_cast<float>(texture->getHeight())
        );
    }

    // Set the state accordingly
    m_sprite->SetDimensions(m_width, m_height);
}
void RenderComponent::Write(std::ofstream& file) 
{
    const std::string& path = m_sprite->GetTexturePath();
    int size = path.size();
    file.write((const char*) &size, sizeof(int));
    file.write((const char*) path.c_str(), size);
    int type = static_cast<int>(m_type);
    file.write((const char*) &type, sizeof(int));

    // Write other important state
    file.write((const char*) &m_width, sizeof(float));
    file.write((const char*) &m_height, sizeof(float));
}

Oasis::Reference<Oasis::Sprite> RenderComponent::GetSprite()
{
    return m_sprite;
}

Oasis::Reference<Oasis::AnimatedSprite> RenderComponent::GetAnimatedSprite()
{
    OASIS_TRAP(m_type == SpriteType::ANIMATED);
    return DynamicCast<Oasis::AnimatedSprite>(m_sprite);
}

void RenderComponent::SetDimensions(float width, float height)
{
    m_sprite->SetDimensions(width, height);
    m_width = width;
    m_height = height;
}

void RenderComponent::SetSourceDimensions(float w, float h)
{
    m_sprite->SetSourceDimensions(w, h);
    if (m_type == SpriteType::ANIMATED)
    {
        GetAnimatedSprite()->SetAnimFrameSize(w, h);
    }
}

void RenderComponent::SetSourcePos(float x, float y)
{
    m_sprite->SetSourcePos(x, y);
}

void RenderComponent::Update(float delta)
{
    m_sprite->SetPos(m_entity->GetX() - Camera::GetX(), m_entity->GetY() - Camera::GetY());
    if (m_type == SpriteType::NORMAL)
    {
        Renderer::DrawSprite(m_sprite);
    }
    if (m_type == SpriteType::ANIMATED)
    {
        Renderer::DrawAnimatedSprite(DynamicCast<AnimatedSprite>(m_sprite));
    }
}