#include "oasis.h"
#include "core/entry.hpp"

#include "game/game.hpp"

Oasis::Configuration Oasis::GetConfiguration()
{
    Oasis::Configuration config;
    config.m_width = 1280;
    config.m_height = 720;
    config.m_name = "Corruption";
    
    // Return the initial state as a callback so it doesn't get instantiated before engine inits
    config.m_initState = []() -> IState* 
    {
        return new Game();
    };

    return config;
}