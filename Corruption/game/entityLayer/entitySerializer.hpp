#pragma once
#include "entity.hpp"

#include <unordered_map>

class EntitySerializer
{
public:
    static void Init();
    static void ExportEntity(Oasis::Reference<Entity> entity, const std::string& filename);
    static Entity* ReadEntity(const std::string& filename);

private:
    static std::unordered_map<int, Component*> s_componentMap;

    /////////////////////////////////////////////////////////////////
    template<typename COMPONENT, int index>
    static void RegisterComponent()
    {
        s_componentMap[index] = new COMPONENT();
        COMPONENT::s_serializerIndex = index;
    };
};