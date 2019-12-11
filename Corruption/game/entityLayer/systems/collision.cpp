#include "collision.hpp"

#include "game/entityLayer/entity.hpp"
#include "game/entityLayer/entityLayer.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"

Oasis::Reference<EntityLayer> Collision::s_game;

void Collision::Init(Oasis::Reference<EntityLayer> game)
{
    s_game = game;
}

Oasis::Reference<Entity> Collision::Colliding(Oasis::Reference<Entity> entity)
{
    // Make sure the entity has a collision component
    auto collision = entity->GetComponent<CollisionComponent>();
    float x = entity->GetX();
    float y = entity->GetY();
    int w = collision->GetWidth();
    int h = collision->GetHeight();

    for (Oasis::Reference<Entity> ent : s_game->GetEntities())
    {
        if (ent == entity) 
        {
            continue;
        }
        if (auto col = ent->GetComponent<CollisionComponent>())
        {
            if (col->Passable()) continue;
            float x2 = ent->GetX();
            float y2 = ent->GetY();
            int w2 = col->GetWidth();
            int h2 = col->GetHeight();
            if (x < x2 + w2 && x + w > x2)
            {
                if (y < y2 + h2 && y + h > y2)
                {
                    return ent;
                }
            }
        }
    }
    return nullptr;
}
