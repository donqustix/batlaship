#include "game_cpu_player.h"
#include "gui_game_state.h"
#include "game_battle_area.h"

#include <cstdlib>
#include <ctime>

using battleship::game::GameCpuPlayer;

GameCpuPlayer::GameCpuPlayer() noexcept : GamePlayer{Types::CPU}
{
    std::srand(std::time(nullptr));
}

void GameCpuPlayer::placeShips(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept
{
    for (unsigned i = 0; i < 5; ++i)
    {
        const bool orientation = std::rand() % 2;

        for (unsigned j = 0; j < ((i < 3) ? 1 : 2); ++j)
        {
            while (!guiGameState.placeShip(gameBattleArea, static_cast<ShipTypes>(i),
                    (5 - i) / 2 *  orientation + std::rand() % (9 - (5 - (5 - i) / 2) *  orientation),
                    (5 - i) / 2 * !orientation + std::rand() % (9 - (5 - (5 - i) / 2) * !orientation),
                        orientation));
        }
    }
}

void GameCpuPlayer::attack(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept
{
    int i, j;
    do
    {
        i = std::rand() % 10;
        j = std::rand() % 10;
    }
    while (gameBattleArea.getGrid(i, j).minShipDistance != 1 || gameBattleArea.getGrid(i, j).destroyed);

    guiGameState.attack(gameBattleArea, i, j);
}

