#include "oasis.h"

#include "mapLayer/mapLayer.hpp"
#include "entityLayer/entityLayer.hpp"

class Game : public Oasis::GameState
{
public:
    virtual void InitLayers() override
    {
        AddLayer(new MapLayer());
        AddLayer(new EntityLayer());
    }
};