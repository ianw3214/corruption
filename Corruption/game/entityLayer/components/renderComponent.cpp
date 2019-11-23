#include "renderComponent.hpp"
using namespace Oasis;

RenderComponent::RenderComponent(const std::string& path)
    : m_sprite(path)
{

}

void RenderComponent::SetDimensions(float width, float height)
{
    m_sprite.SetDimensions(width, height);
}


void RenderComponent::Update()
{
    m_sprite.SetPos(m_entity->GetX(), m_entity->GetY());
    Renderer::DrawSprite(m_sprite);
}