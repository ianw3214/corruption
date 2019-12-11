#pragma once
#include "oasis.h"

class Entity;
class EntityLayer;
class Collision
{
public:
    static void Init(Oasis::Reference<EntityLayer> game);
    
    static Oasis::Reference<Entity> Colliding(Oasis::Reference<Entity> entity);
private:
    static Oasis::Reference<EntityLayer> s_game;
};