#pragma once
#include "oasis.h"

constexpr float kPlayerAttackCooldown = 0.4f;
constexpr float kProjectileWidth = 60.f;
constexpr float kProjectileHeight = 60.f;

class Entity;
class EntityLayer;
class PlayerController
{
public:
    static void SetPlayer(Oasis::Reference<Entity> entity);
    static void SetGame(Oasis::Reference<EntityLayer> layer);

    static bool OnEvent(const Oasis::Event& event);
    static void Update(float delta);
    static void UpdateAnimations(float delta);
private:
    static Oasis::Reference<Entity> s_player;
    static Oasis::Reference<EntityLayer> s_game;

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
    static float s_attackCooldown;
};