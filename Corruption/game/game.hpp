#include "oasis.h"

#include "entityLayer/entityLayer.hpp"

class MainGameLayer : public Oasis::GameStateLayer
{
public:
    virtual void Init() override;
    virtual void Close() override;

    virtual bool HandleEvent(const Oasis::Event& event) override;
    virtual void Update()  override;
private:
};

class Game : public Oasis::GameState
{
public:
    virtual void InitLayers() override
    {
        AddLayer(new MainGameLayer());
        AddLayer(new EntityLayer());
    }
};