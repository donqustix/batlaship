#ifndef GUISTATE_H
#define GUISTATE_H

#include <SDL2/SDL_events.h>

namespace battleship
{
    namespace engine
    {
        class Graphics;

        class GuiState
        {
            unsigned state;

        public:
            explicit GuiState(unsigned state) noexcept : state{state} {}
            virtual ~GuiState() = default;

            virtual void render(const Graphics& graphics, unsigned delta) noexcept = 0;
            virtual void handle(const SDL_Event& event) noexcept = 0;

            auto getState() const noexcept {return state;}
        };
    }
}

#endif
