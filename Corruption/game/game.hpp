#include "oasis.h"

#include "mapLayer/mapLayer.hpp"
#include "entityLayer/entityLayer.hpp"
#include "debugLayer/debugLayer.hpp"
#include "editorLayer/editorLayer.hpp"

class Game : public Oasis::GameState
{
public:
    // Helper functions to access game layers
    static Oasis::Reference<EntityLayer> GetEntityLayer() { return s_entityLayer; }
    static Oasis::Reference<MapLayer> GetMapLayer() { return s_mapLayer; }

    virtual void InitLayers() override
    {
        // Layers are added top -> bottom
        AddLayer(new EditorLayer());
        s_mapLayer = Oasis::DynamicCast<MapLayer>(AddLayer(new MapLayer()));
        s_entityLayer = Oasis::DynamicCast<EntityLayer>(AddLayer(new EntityLayer()));
        AddLayer(new DebugLayer());
    }
private:
    static Oasis::Reference<EntityLayer> s_entityLayer;
    static Oasis::Reference<MapLayer> s_mapLayer;
};