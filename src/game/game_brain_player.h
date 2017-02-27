#ifndef GAMEBRAINPLAYER_H
#define GAMEBRAINPLAYER_H

#include "game_player.h"

namespace battleship
{
    namespace game
    {
        class GuiGameStateManager;

        class GameBrainPlayer : public GamePlayer
        {
            GuiGameStateManager* guiGameStateManager;

        public:
            explicit GameBrainPlayer(GuiGameStateManager& guiGameStateManager) noexcept :
                GamePlayer{Types::BRAIN}, guiGameStateManager{&guiGameStateManager} {}

            void placeShips(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept override;
            void attack(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept override;
        };
    }
}

#endif
