#include "renderComponent.hpp"
using namespace Oasis;

RenderComponent::RenderComponent(const std::string& path, SpriteType type)
    : m_type(type)
{
    if (type == SpriteType::NORMAL)
    {
        m_sprite = new Sprite(path);
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
    m_sprite->SetPos(m_entity->GetX(), m_entity->GetY());
    if (m_type == SpriteType::NORMAL)
    {
        Renderer::DrawSprite(m_sprite);
    }
    if (m_type == SpriteType::ANIMATED)
    {
        Renderer::DrawAnimatedSprite(DynamicCast<AnimatedSprite>(m_sprite));
    }
}