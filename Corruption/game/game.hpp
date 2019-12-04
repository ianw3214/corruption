#include "oasis.h"

#include "mapLayer/mapLayer.hpp"
#include "entityLayer/entityLayer.hpp"
#include "debugLayer/debugLayer.hpp"

class Game : public Oasis::GameState
{
public:
    // Make sure these aren't called inside init functions otherwise reference will be null
    // Helper functions
    static Oasis::Reference<EntityLayer> GetEntityLayer() { return s_entityLayer; }

    virtual void InitLayers() override
    {
        AddLayer(new MapLayer());
        s_entityLayer = Oasis::DynamicCast<EntityLayer>(AddLayer(new EntityLayer()));
        AddLayer(new DebugLayer());
    }
private:
    static Oasis::Reference<EntityLayer> s_entityLayer;
};