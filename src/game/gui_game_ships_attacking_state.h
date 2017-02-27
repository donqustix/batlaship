#ifndef GUIGAMESHIPSATTACKINGSTATE_H
#define GUIGAMESHIPSATTACKINGSTATE_H

#include "engine/gui_state.h"

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
        class GuiGameState;
        class GameBattleArea;

        class GuiGameShipsAttackingState : public engine::GuiState
        {
            std::unique_ptr<engine::GuiButton> buttonFire;
            const engine::Font* font;

            struct
            {
                int i = 0;
                int j = 0;
            } cursor;

            GuiGameState* guiGameState;
            GameBattleArea* gameBattleArea;

        public:
            explicit GuiGameShipsAttackingState(Application& application);
            GuiGameShipsAttackingState(GuiGameShipsAttackingState&&) noexcept;
            ~GuiGameShipsAttackingState();

            GuiGameShipsAttackingState& operator=(GuiGameShipsAttackingState&&) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta) noexcept override;
            void handle(const SDL_Event& event) noexcept override;

            void setGameBattleArea(GameBattleArea& gameBattleArea) noexcept
            {
                this->gameBattleArea = &gameBattleArea;
            }
        };
    }
}

#endif
