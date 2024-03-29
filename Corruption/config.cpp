#include "oasis.h"
#include "core/entry.hpp"

#include "title/title.hpp"
#include "game/game.hpp"
#include "util/timer.hpp"

Oasis::Configuration Oasis::GetConfiguration()
{
    Oasis::Configuration config;
    config.m_width = 1280;
    config.m_height = 960;
    config.m_name = "Corruption";
    
    // Return the initial state as a callback so it doesn't get instantiated before engine inits
    config.m_initState = []() -> IState* 
    {
        return new Title();
    };

    return config;
}