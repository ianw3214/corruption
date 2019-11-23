#pragma once
#include "oasis.h"

#include <vector>

#include "game/entityLayer/entity.hpp"

class EntityLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;

    // AddPlayer also calls AddEntity, don't need to call twice
    Oasis::Reference<Entity> AddPlayer(Entity * entity);    
    Oasis::Reference<Entity> AddEntity(Entity * entity);
private:
    std::vector<Oasis::Owned<Entity>> m_entities;

    // Keep explicit reference to the player since it is used so often
    Oasis::Reference<Entity> m_player;
};