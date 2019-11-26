#pragma once
#include "oasis.h"

class Entity;
class Camera
{
public:
    static void SetPlayer(Oasis::Reference<Entity> entity);

    static float GetX();
    static float GetY();
    static void Update(float delta);
private:
    static Oasis::Reference<Entity> s_player;

    static float s_cameraX;
    static float s_cameraY;
};