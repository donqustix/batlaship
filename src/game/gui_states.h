#ifndef GUISTATES_H
#define GUISTATES_H

namespace battleship
{
    namespace game
    {
        enum class GuiStates
        {
            MAIN_MENU,
            GAME_MODE_SELECTION,
            GAME_STATE,
            GAME_SHIPS_PLACING,
            GAME_SHIPS_ATTACKING,
            OPTIONS
        };
    }
}

#endif
