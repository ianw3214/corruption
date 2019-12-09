#include "entitySerializer.hpp"

#include "components/renderComponent.hpp"
#include "components/collisionComponent.hpp"
#include "components/interactableComponent.hpp"
#include "components/healthComponent.hpp"
#include "components/projectileComponent.hpp"

std::unordered_map<int, Component*> EntitySerializer::s_componentMap;

void EntitySerializer::Init()
{
    s_componentMap.clear();

    RegisterComponent<RenderComponent, 0>();
    RegisterComponent<CollisionComponent, 1>();
    RegisterComponent<InteractableComponent, 2>();
    RegisterComponent<HealthComponent, 3>();
    RegisterComponent<ProjectileComponent, 4>();
}

void EntitySerializer::ExportEntity(Oasis::Reference<Entity> entity, const std::string& filename)
{
    std::ofstream file("res/entities/" + filename, std::ios::binary);
    if (file.is_open())
    {
        // WRITE THE NUMBER OF COMPONENTS IN THE FILE
        int num_components = entity->m_components.size();
        file.write((const char*) &num_components, sizeof(int));
        // WRITE ENTITY DATA (X/Y)
        float x = entity->GetX();
        float y = entity->GetY();
        file.write((const char*) &x, sizeof(float));
        file.write((const char*) &y, sizeof(float));
        // THEN WRITE EVERY COMPONENT
        for (Oasis::Reference<Component> component : entity->m_components)
        {
            // FIRST WRITE THE INDEX OF THE COMPONENT
            int index = component->GetIndex();
            file.write((const char*) &index, sizeof(int));
            // THEN WRITE THE ACTUAL COMPONENT DATA
            component->Write(file);
        }
    }
    else
    {
        Oasis::Console::Print("Entity export failed: " + filename);
    }
}

Entity* EntitySerializer::ReadEntity(const std::string& filename)
{
    Entity * entity = new Entity();
    std::ifstream file("res/entities/" + filename, std::ios::binary);
    if (file.is_open())
    {
        // FIRST READ THE NUMBER OF COMPONENTS THAT EXIST IN THE ENTITY
        int num_components;
        file.read((char*) &num_components, sizeof(int));
        // READ ENTITY DATA (X/Y)
        float x;
        float y;
        file.read((char*) &x, sizeof(float));
        file.read((char*) &y, sizeof(float));
        entity->SetX(x);
        entity->SetY(y);
        // THEN READ EACH COMPONENT
        for (int i = 0; i < num_components; ++i)
        {
            // FIRST READ EACH INDEX
            int index;
            file.read((char*)&index, sizeof(int));
            // THEN READ THE COMPONENT BASED ON INDEX DATA
            Component * comp = s_componentMap[index]->Clone();
            comp->Read(file);
            entity->AddComponent(comp);
        }
    }
    else
    {
        Oasis::Console::Print("Entity read failed: " + filename);
    }
    return entity;
}