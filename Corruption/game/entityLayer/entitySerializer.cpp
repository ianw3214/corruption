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
    ExportEntity(entity, file);
}

void EntitySerializer::ExportEntity(Oasis::Reference<Entity> entity, std::ofstream& file)
{
    if (file.is_open())
    {
        // WRITE THE NUMBER OF COMPONENTS IN THE FILE
        int num_components = entity->m_components.size();
        file.write((const char*) &num_components, sizeof(int));
        // WRITE ENTITY DATA (SERIALIZED X/Y)
        float serializedX = entity->GetSerializedX();
        float serializedY = entity->GetSerializedY();
        file.write((const char*) &serializedX, sizeof(float));
        file.write((const char*) &serializedY, sizeof(float));
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
        // TODO: Hopefully more detailed logging
        Oasis::Console::Print("Entity export failed");
    }
}

Entity* EntitySerializer::ReadEntity(const std::string& filename)
{
    std::ifstream file("res/entities/" + filename, std::ios::binary);
    return ReadEntity(file);
}

Entity* EntitySerializer::ReadEntity(std::ifstream& file)
{
    Entity * entity = new Entity();
    if (file.is_open())
    {
        // FIRST READ THE NUMBER OF COMPONENTS THAT EXIST IN THE ENTITY
        int num_components;
        file.read((char*) &num_components, sizeof(int));
        // READ SERIALIZED X/Y (we don't care about actual x/y)
        float x;
        float y;
        file.read((char*) &x, sizeof(float));
        file.read((char*) &y, sizeof(float));
        entity->SetSerializedX(x);
        entity->SetSerializedY(y);
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
        // TODO: Hopefully better logging
        Oasis::Console::Print("Entity read failed");
    }
    entity->MarkSerialized();
    entity->SetX(entity->m_serializedX);
    entity->SetY(entity->m_serializedY);
    return entity;
}