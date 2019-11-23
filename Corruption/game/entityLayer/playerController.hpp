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

    // State variables
    static bool m_upPressed;
    static bool m_downPressed;
    static bool m_leftPressed;
    static bool m_rightPressed;
};