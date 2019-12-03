#pragma once
#include "oasis.h"

class Entity;
class PlayerController
{
public:
    static void SetPlayer(Oasis::Reference<Entity> entity);

    static bool OnEvent(const Oasis::Event& event);
    static void Update(float delta);

    static void DEBUG();
private:
    static Oasis::Reference<Entity> s_player;

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    // State variables
    static bool s_upHeld;
    static bool s_downHeld;
    static bool s_leftHeld;
    static bool s_rightHeld;
    static Direction s_direction;
};