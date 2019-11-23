#include "oasis.h"
#include "game/entityLayer/entity.hpp"

// Render component acts as a wrapper around the sprite class
class RenderComponent : public Component
{
public:
    RenderComponent(const std::string& path);
    void SetDimensions(float width, float height);

    virtual void Update() override;
private:
    Oasis::Sprite m_sprite;
};