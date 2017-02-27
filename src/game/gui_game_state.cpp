#include "gui_game_state.h"
#include "gui_game_state_manager.h"
#include "gui_states.h"
#include "gui_game_ships_placing_state.h"
#include "game_battle_area.h"
#include "application.h"
#include "game_brain_player.h"
#include "gui_button_factory.h"

#include "engine/graphics.h"
#include "engine/font.h"
#include "engine/resource_manager.h"
#include "engine/gui_button.h"
#include "engine/texture_atlas.h"

using battleship::game::GuiGameState;

using battleship::engine::Graphics;
using battleship::engine::Font;
using battleship::engine::GuiButton;
using battleship::engine::TextureAtlas;

namespace
{
    constexpr int BUTTON_SCALE = 2;
    constexpr int BATTLE_AREA_SCALE = 14;
    constexpr int BATTLE_AREA_GRID_SIZE =
        battleship::game::GameBattleArea::Grid::calculateSize(BATTLE_AREA_SCALE);
    constexpr int BATTLE_AREA_WIDTH = BATTLE_AREA_GRID_SIZE * 10;

    void resetShips(std::array<battleship::game::Ship, 5>& ships) noexcept
    {
        ships[0] = {battleship::game::ShipTypes::CARRIER,    1, 5};
        ships[1] = {battleship::game::ShipTypes::BATTLESHIP, 1, 4};
        ships[2] = {battleship::game::ShipTypes::CRUISER,    1, 3};
        ships[3] = {battleship::game::ShipTypes::SUBMARINE,  2, 2};
        ships[4] = {battleship::game::ShipTypes::DESTROYER,  2, 1};
    }
}

GuiGameState::GuiGameState(Application& application) :
    GuiState{static_cast<unsigned>(GuiStates::GAME_STATE)},
    battleArea1{std::make_unique<GameBattleArea>(application.getResourceManager())},
    battleArea2{std::make_unique<GameBattleArea>(application.getResourceManager())},
    guiGameStateManager{&application.getGuiGameStateManager()}
{
    font = application.getResourceManager().get<Font>("font_24");

    const GuiButtonFactory guiButtonFactory{application.getResourceManager()};

    const Graphics& graphics = application.getGraphics();

    const int virtualWidth  = graphics.getVirtualWidth();

    const TextureAtlas& textureAtlasButton = guiButtonFactory.getTextureAtlas();

    const int buttonWidth  = textureAtlasButton.getArea("released_button", true)->w * BUTTON_SCALE;

    buttonMainMenu = guiButtonFactory.createButton("main menu", 
            [this, &application] {
                battleArea1->reset();
                battleArea2->reset();
                ::resetShips(ships);
                cursorAttack.i = -1;
                currentState = States::PLAYER1_PLACES_SHIPS;
                application.getGuiGameStateManager().
                    setCurrentState(static_cast<unsigned>(GuiStates::MAIN_MENU));
            }, (virtualWidth - buttonWidth) >> 1, 80 + BATTLE_AREA_WIDTH + 50, BUTTON_SCALE);
}

GuiGameState::GuiGameState(GuiGameState&&) noexcept = default;

GuiGameState::~GuiGameState() = default;

GuiGameState& GuiGameState::operator=(GuiGameState&&) noexcept = default;

void GuiGameState::render(const Graphics& graphics, unsigned delta) noexcept
{
    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    graphics.setColor(150, 150, 150);
    graphics.fillRect(0, 0, virtualWidth, virtualHeight);

    switch (currentState)
    {
        case States::PLAYER1_PLACES_SHIPS: {
            if (battleArea1->getShips())
                currentState = States::PLAYER2_PLACES_SHIPS;
            else
                player1->placeShips(*this, *battleArea1);
        } break;
        case States::PLAYER2_PLACES_SHIPS: {
            if (battleArea2->getShips())
                currentState = States::PLAYER1_ATTACKS;
            else
            {
                ::resetShips(ships);
                player2->placeShips(*this, *battleArea2);
            }
        } break;
        default: {
            if (isGameOver())
            {
                const auto text = std::string{"player #"} + (battleArea1->isNoShips() ? '2' : '1') + " won!";
                graphics.drawText(*font, text, virtualWidth / 2 - text.length() * font->getSize() / 4,
                    80 + BATTLE_AREA_WIDTH, {80, 0, 0, 255});
            }
            else
            {
                static float time = 0.0F;
                if (time >= 60.0F)
                {
                    time = 0.0F;
                    switch (currentState)
                    {
                        case States::PLAYER1_ATTACKS: {
                            player1->attack(*this, *battleArea2);
                            currentState = States::PLAYER2_ATTACKS;
                        } break;
                        case States::PLAYER2_ATTACKS: {
                            player2->attack(*this, *battleArea1);
                            currentState = States::PLAYER1_ATTACKS;
                        } break;
                        default:break;
                    }
                    cursorAttack.player = currentState == States::PLAYER2_ATTACKS;
                }
                else time += 0.05F * delta;

                const auto text = std::string{"player #"} +
                    (currentState == States::PLAYER1_ATTACKS ? '1' : '2') + " is moving...";
                graphics.drawText(*font, text, virtualWidth / 2 - text.length() * font->getSize() / 4,
                    80 + BATTLE_AREA_WIDTH, {80, 0, 0, 255});
            }
        } break;
    }

    battleArea1->render(graphics, delta, 10, 50, BATTLE_AREA_SCALE,
            player2->getType() == GamePlayer::Types::CPU);
    battleArea2->render(graphics, delta, virtualWidth - BATTLE_AREA_WIDTH - 10, 50, BATTLE_AREA_SCALE,
            player1->getType() == GamePlayer::Types::CPU);

    buttonMainMenu->render(graphics);

    if (cursorAttack.i != -1)
    {
        static bool enabled = false;
        static float time = 0.0F;
        if (time >= 8.0F)
        {
            time = 0.0F;
            enabled = !enabled;
        }
        if (enabled)
        {
            graphics.setColor(255, 0, 0);
            graphics.fillRect((cursorAttack.player ? (virtualWidth - BATTLE_AREA_WIDTH - 10) : 10) +
                cursorAttack.j * BATTLE_AREA_GRID_SIZE, 50 + cursorAttack.i * BATTLE_AREA_GRID_SIZE,
                        BATTLE_AREA_GRID_SIZE, BATTLE_AREA_GRID_SIZE);
        }
        time += 0.05F * delta;
    }
}

void GuiGameState::handle(const SDL_Event& event) noexcept
{
    buttonMainMenu->handle(event);
}

bool GuiGameState::isGameOver() const noexcept
{
    return battleArea1->isNoShips() || battleArea2->isNoShips();
}

bool GuiGameState::placeShip(GameBattleArea& gameBattleArea, ShipTypes shipType, int i, int j,
        bool orientation) noexcept
{
    if (!ships[static_cast<unsigned>(shipType)].amount)
        return false;

    const int length = ships[static_cast<unsigned>(shipType)].length;
    for (int k = 0; k < length; ++k)
    {
        const int gi = orientation ?     i + k - length / 2 : i;
        const int gj = orientation ? j : j + k - length / 2;

        if (gameBattleArea.getGrid(gi, gj).ship)
            return false;
    }
    if (length == 1)
        gameBattleArea.setShip("single_ship", i, j);
    else
    {
        for (int k = 0; k < length; ++k)
        {
            const auto shipType = std::string{orientation ? "ver_ship_" : "hor_ship_"} +
                (!k ? "1" : k < length - 1 ? "2" : "3");
            gameBattleArea.setShip(shipType, orientation
                    ? i + k - length / 2 : i, orientation ? j : j + k - length / 2);
        }
    }
    --ships[static_cast<unsigned>(shipType)].amount;

    return true;
}

bool GuiGameState::attack(GameBattleArea& gameBattleArea, int i, int j) noexcept
{
    const bool tempShip = gameBattleArea.getGrid(i, j).ship;

    gameBattleArea.destroyShip(i, j);
    cursorAttack.i = i;
    cursorAttack.j = j;

    return tempShip != gameBattleArea.getGrid(i, j).ship;
}

void GuiGameState::reset(GameBattleArea& gameBattleArea) noexcept
{
    if (currentState != States::PLAYER1_ATTACKS &&
        currentState != States::PLAYER2_ATTACKS)
    {
        gameBattleArea.reset();
        ::resetShips(ships);
    }
}

