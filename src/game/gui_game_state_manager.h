#ifndef GUIGAMESTATEMANAGER_H
#define GUIGAMESTATEMANAGER_H

#include "engine/gui_state_manager.h"

namespace battleship
{
    namespace game
    {
        class Application;

        class GuiGameStateManager : public engine::GuiStateManager
        {
        public:
            void init(Application& application);
        };
    }
}

#endif
