#pragma once
#include "oasis.h"
#include "game/entityLayer/entity.hpp"

#include <vector>
#include <functional>

class InteractableComponent;
class InteractionManager
{
public:
    static void Init();
    static void Shutdown();

    static bool OnEvent(const Oasis::Event& e);
    static void RegisterInteractableComponent(Oasis::Reference<InteractableComponent> comp);
private:
    static std::vector<Oasis::Reference<InteractableComponent>> s_interactions;
};

// Assumes all collisions are squares
// Can consider other shapes in the future
class InteractableComponent : public Component
{
public:
    InteractableComponent(std::function<void()> f = [](){});

    void SetInteractFunc(std::function<void()> func);
    inline std::function<void()> GetInteractFunc() { return m_func; }

    // virtual void Update(float delta) override;
private:
    std::function<void()> m_func;
};