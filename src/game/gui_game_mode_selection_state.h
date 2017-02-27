#ifndef GUIGAMEMODESELECTIONSTATE_H
#define GUIGAMEMODESELECTIONSTATE_H

#include "engine/gui_state.h"

#include <vector>
#include <memory>

namespace battleship
{
    namespace engine
    {
        class Font;
        class GuiButton;
    }

    namespace game
    {
        class Application;

        class GuiGameModeSelectionState : public engine::GuiState
        {
            std::vector<std::unique_ptr<engine::GuiButton>> buttons;
            const engine::Font* font;

        public:
            explicit GuiGameModeSelectionState(Application& application);
            GuiGameModeSelectionState(GuiGameModeSelectionState&&) noexcept;
            ~GuiGameModeSelectionState();

            GuiGameModeSelectionState& operator=(GuiGameModeSelectionState&& guiGameModeSelectionState) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta) noexcept override;
            void handle(const SDL_Event& event) noexcept override;
        };
    }
}

#endif
