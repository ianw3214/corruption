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

    Oasis::Reference<Entity> AddEntity(Entity * entity);
private:
    std::vector<Oasis::Owned<Entity>> m_entities;
};