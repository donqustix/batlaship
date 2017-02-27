#include "game_brain_player.h"
#include "gui_game_state_manager.h"
#include "gui_game_ships_placing_state.h"
#include "gui_game_ships_attacking_state.h"
#include "gui_states.h"

using battleship::game::GameBrainPlayer;

void GameBrainPlayer::placeShips(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept
{
    const auto guiGameShipsPlacingState = guiGameStateManager->findState<GuiGameShipsPlacingState>(
            static_cast<unsigned>(GuiStates::GAME_SHIPS_PLACING));
    guiGameShipsPlacingState->setGameBattleArea(gameBattleArea);
    guiGameStateManager->setCurrentState(static_cast<unsigned>(GuiStates::GAME_SHIPS_PLACING));
}

void GameBrainPlayer::attack(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept
{
    const auto guiGameShipsAttackingState = guiGameStateManager->findState<GuiGameShipsAttackingState>(
            static_cast<unsigned>(GuiStates::GAME_SHIPS_ATTACKING));
    guiGameShipsAttackingState->setGameBattleArea(gameBattleArea);
    guiGameStateManager->setCurrentState(static_cast<unsigned>(GuiStates::GAME_SHIPS_ATTACKING));
}

