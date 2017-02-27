#include "gui_game_state_manager.h"
#include "gui_main_menu_state.h"
#include "gui_game_mode_selection_state.h"
#include "gui_game_state.h"
#include "gui_game_ships_placing_state.h"
#include "gui_game_ships_attacking_state.h"

using battleship::game::GuiGameStateManager;

void GuiGameStateManager::init(Application& application)
{
    this->addState<GuiMainMenuState>(application);
    this->addState<GuiGameModeSelectionState>(application);
    this->addState<GuiGameState>(application);
    this->addState<GuiGameShipsPlacingState>(application);
    this->addState<GuiGameShipsAttackingState>(application);
}

