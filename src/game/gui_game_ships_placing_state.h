#ifndef GUIGAMESHIPSPLACINGSTATE_H
#define GUIGAMESHIPSPLACINGSTATE_H

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
        class GameBattleArea;
        class Application;
        class GuiGameState;
        enum class ShipTypes;

        class GuiGameShipsPlacingState : public engine::GuiState
        {
            std::vector<std::unique_ptr<engine::GuiButton>> buttons;
            const engine::Font* font;

            struct
            { 
                ShipTypes shipType;
                int i;
                int j;
                bool orientation = false;
                bool enabled = false;
            } cursor;

            GuiGameState* guiGameState;
            GameBattleArea* gameBattleArea;
            bool showInfo = false;

            void updateButtons() noexcept;

        public:
            explicit GuiGameShipsPlacingState(Application& application);
            GuiGameShipsPlacingState(GuiGameShipsPlacingState&&) noexcept;
            ~GuiGameShipsPlacingState();

            GuiGameShipsPlacingState& operator=(GuiGameShipsPlacingState&&) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta) noexcept override;
            void handle(const SDL_Event& event) noexcept override;

            void setGameBattleArea(GameBattleArea& gameBattleArea) noexcept
            {
                this->gameBattleArea = &gameBattleArea;
                updateButtons();
            }
        };
    }
}

#endif
