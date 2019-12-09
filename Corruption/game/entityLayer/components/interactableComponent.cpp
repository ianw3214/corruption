#include "interactableComponent.hpp"
IMPL_COMPONENT(InteractableComponent);

#include "game/entityLayer/camera.hpp"
#include "game/entityLayer/components/collisionComponent.hpp"

std::vector<Oasis::Reference<InteractableComponent>> InteractionManager::s_interactions;

void InteractionManager::Init()
{
    // Do nothing I think
}

void InteractionManager::Shutdown()
{
    // Do nothing I think
}

bool InteractionManager::OnEvent(const Oasis::Event& e)
{
    if (e.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        const Oasis::MousePressedEvent& mouseEvent = dynamic_cast<const Oasis::MousePressedEvent&>(e);
        float mouseX = static_cast<float>(mouseEvent.GetX()) + Camera::GetX();
        // This is a super hack because SDL uses top left as origin but I used bottom left :/
        float mouseY = Oasis::WindowService::WindowHeight() - static_cast<float>(mouseEvent.GetY()) + Camera::GetY();

        for (auto ref : s_interactions)
        {
            Oasis::Reference<Entity> entity = ref->GetEntity();
            if (entity)
            {
                if (Oasis::Reference<CollisionComponent> col = entity->GetComponent<CollisionComponent>())
                {
                    float x = entity->GetX();
                    float y = entity->GetY();
                    float w = static_cast<float>(col->GetWidth());
                    float h = static_cast<float>(col->GetHeight());
                    if (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h)
                    {
                        ref->GetInteractFunc()();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void InteractionManager::RegisterInteractableComponent(Oasis::Reference<InteractableComponent> comp)
{
    s_interactions.push_back(comp);
}


InteractableComponent::InteractableComponent(std::function<void()> f)
    : m_func(f)
{
    InteractionManager::RegisterInteractableComponent(this);
}

void InteractableComponent::SetInteractFunc(std::function<void()> func)
{
    m_func = func;
}