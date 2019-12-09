#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

enum class SpriteType
{
    NORMAL = 0,
    ANIMATED = 1
};

// Render component acts as a wrapper around the sprite class
// NOTE: Assumes all entities are affected by camera - if NOT the case need to adjust in future
class RenderComponent : public Component
{
public:
    REGISTER_COMPONENT(RenderComponent);
    
    RenderComponent();
    RenderComponent(const std::string& path, SpriteType type = SpriteType::NORMAL);

    virtual void Read(std::ifstream& file) override;
    virtual void Write(std::ofstream& file) override;

    SpriteType GetType() const { return m_type; }
    Oasis::Reference<Oasis::Sprite> GetSprite();
    Oasis::Reference<Oasis::AnimatedSprite> GetAnimatedSprite();

    void SetDimensions(float width, float height);
    void SetSourceDimensions(float w, float h);
    void SetSourcePos(float x, float y);

    virtual void Update(float delta) override;
private:
    SpriteType m_type;
    Oasis::Owned<Oasis::Sprite> m_sprite;

    // State that's useful for things
    float m_width;
    float m_height;
};