#ifndef GUIMAINMENUSTATE_H
#define GUIMAINMENUSTATE_H

#include "engine/gui_state.h"

#include <vector>
#include <memory>

namespace battleship
{
    namespace engine
    {
        class GuiButton;
        class Font;
    }

    namespace game
    {
        class Application;

        class GuiMainMenuState : public engine::GuiState
        {
            std::vector<std::unique_ptr<engine::GuiButton>> buttons;
            const engine::Font* fontLogo;

        public:
            explicit GuiMainMenuState(Application& application);
            GuiMainMenuState(GuiMainMenuState&&) noexcept;
            ~GuiMainMenuState();

            GuiMainMenuState& operator=(GuiMainMenuState&& guiMainMenuState) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta) noexcept override;
            void handle(const SDL_Event& event) noexcept override;
        };
    }
}

#endif
