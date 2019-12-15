#include "title.hpp"
#include "game/game.hpp"

#include "game/entityLayer/entitySerializer.hpp"

void Title::Init()
{
    // Initialize any global resources because the title will only be loaded once at the start
    Oasis::TextRenderer::LoadFont("res/fonts/Munro.ttf");

    // Initialize some gameplace systems that are 'global'
    // TODO: Maybe put these in the systems folder
    EntitySerializer::Init();
}

void Title::Close()
{

}

void Title::OnEvent(const Oasis::Event& event)
{

}

void Title::Update()
{
    // Change state directly from the beginning
    Oasis::StateManager::ChangeState(new Game());
}
