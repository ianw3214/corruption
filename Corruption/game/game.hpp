#include "oasis.h"

#include "mapLayer/mapLayer.hpp"
#include "entityLayer/entityLayer.hpp"
#include "UILayer/UILayer.hpp"
#include "debugLayer/debugLayer.hpp"
#include "editorLayer/editorLayer.hpp"

class Game : public Oasis::GameState
{
public:
    // Helper functions to access game layers
    static Oasis::Reference<EntityLayer> GetEntityLayer() { return s_entityLayer; }
    static Oasis::Reference<MapLayer> GetMapLayer() { return s_mapLayer; }
    static Oasis::Reference<EditorLayer> GetEditorLayer() { return s_editorLayer; }

    virtual void InitLayers() override
    {
        // Layers are added bottom -> top
        s_mapLayer = Oasis::DynamicCast<MapLayer>(AddLayer(new MapLayer()));
        s_entityLayer = Oasis::DynamicCast<EntityLayer>(AddLayer(new EntityLayer()));
        AddLayer(new UILayer());
        s_editorLayer = Oasis::DynamicCast<EditorLayer>(AddLayer(new EditorLayer()));
        AddLayer(new DebugLayer());

        s_mapLayer->LoadSectors();
    }
private:
    static Oasis::Reference<EntityLayer> s_entityLayer;
    static Oasis::Reference<MapLayer> s_mapLayer;
    static Oasis::Reference<EditorLayer> s_editorLayer;
};