#ifndef GAMECPUPLAYER_H
#define GAMECPUPLAYER_H

#include "game_player.h"

namespace battleship
{
    namespace game
    {
        class GameCpuPlayer : public GamePlayer
        {
        public:
            GameCpuPlayer() noexcept;

            void placeShips(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept override;
            void attack(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept override;
        };
    }
}

#endif
