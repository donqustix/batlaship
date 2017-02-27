#include "gui_game_ships_placing_state.h"
#include "gui_states.h"
#include "gui_game_state_manager.h"
#include "gui_game_state.h"
#include "game_battle_area.h"
#include "application.h"
#include "gui_button_factory.h"

#include "engine/graphics.h"
#include "engine/resource_manager.h"
#include "engine/gui_button.h"
#include "engine/font.h"
#include "engine/texture_atlas.h"

#include <utility>
#include <functional>

using battleship::game::GuiGameShipsPlacingState;

using battleship::engine::Graphics;
using battleship::engine::ResourceManager;
using battleship::engine::TextureAtlas;
using battleship::engine::Font;
using battleship::engine::GuiButton;

namespace
{
    constexpr int BUTTON_SCALE = 2;
    constexpr int BATTLE_AREA_X = 15;
    constexpr int BATTLE_AREA_Y = 15;
    constexpr int BATTLE_AREA_SCALE = 14;
    constexpr int BATTLE_AREA_GRID_SIZE =
        battleship::game::GameBattleArea::Grid::calculateSize(BATTLE_AREA_SCALE);
    constexpr int BATTLE_AREA_WIDTH = BATTLE_AREA_GRID_SIZE * 10;

    const std::string buttonNames[]
    {
        "carrier", "battleship", "cruiser", "submarine", "destroyer"
    };
}

GuiGameShipsPlacingState::GuiGameShipsPlacingState(Application& application) :
    GuiState{static_cast<unsigned>(GuiStates::GAME_SHIPS_PLACING)}
{
    guiGameState = application.getGuiGameStateManager().
        findState<GuiGameState>(static_cast<unsigned>(GuiStates::GAME_STATE));

    ResourceManager& resourceManager = application.getResourceManager();
    font = resourceManager.get<Font>("font_24");

    const GuiButtonFactory guiButtonFactory{resourceManager};

    const Graphics& graphics = application.getGraphics();

    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    const TextureAtlas& textureAtlasButton = guiButtonFactory.getTextureAtlas();

    const int buttonWidth  = textureAtlasButton.getArea("pressed_button", true)->w * BUTTON_SCALE;
    const int buttonHeight = textureAtlasButton.getArea("pressed_button", true)->h * BUTTON_SCALE;

    auto initCursor = [this] (const Ship& ship) {
        cursor.shipType = ship.type;
        cursor.i = cursor.j = 5;
        cursor.enabled = true;
    };

    const std::array<Ship, 5>& ships = guiGameState->getShips();

    for (unsigned i = 0; i < 5; ++i)
    {
        buttons.push_back(guiButtonFactory.createButton(buttonNames[i] + " x" +
            std::to_string(ships[i].amount), std::bind(initCursor, ships[i]),
                virtualWidth - buttonWidth - 50, 60 + (5 + buttonHeight) * i, BUTTON_SCALE));
    }

    buttons.push_back(guiButtonFactory.createButton("main menu",
                [this, &application] {
                    cursor.enabled = showInfo = false;
                    guiGameState->reset(*gameBattleArea);
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::MAIN_MENU));

                }, 10, virtualHeight - buttonHeight - 10, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("next",
                [this, &application] {
                    if (gameBattleArea->getShips())
                    {
                        cursor.enabled = showInfo = false;
                        application.getGuiGameStateManager().
                            setCurrentState(static_cast<unsigned>(GuiStates::GAME_STATE));
                    }
                    else
                        showInfo = true;

                }, virtualWidth - buttonWidth - 10, virtualHeight - buttonHeight - 10, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("reset",
                [this] {
                    cursor.enabled = false;
                    guiGameState->reset(*gameBattleArea);
                    updateButtons();

                }, virtualWidth - buttonWidth - 50, 85 + buttonHeight * 5, BUTTON_SCALE));
}

GuiGameShipsPlacingState::GuiGameShipsPlacingState(GuiGameShipsPlacingState&&) noexcept = default;

GuiGameShipsPlacingState::~GuiGameShipsPlacingState() = default;

GuiGameShipsPlacingState& GuiGameShipsPlacingState::operator=(GuiGameShipsPlacingState&&) noexcept = default;

void GuiGameShipsPlacingState::render(const Graphics& graphics, unsigned delta) noexcept
{
    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    graphics.setColor(150, 150, 150);
    graphics.fillRect(0, 0, virtualWidth, virtualHeight);

    gameBattleArea->render(graphics, delta, BATTLE_AREA_X, BATTLE_AREA_Y, BATTLE_AREA_SCALE, true);

    static float time = 0.0F;
    if (time >= 10.0F)
        time = 0.0F;
    time += 0.01F * delta;

    if (cursor.enabled)
    {
        const Ship& ship = guiGameState->getShips()[static_cast<unsigned>(cursor.shipType)];

        graphics.setColor(time * 25.5F, 0, 0);
        for (int i = 0; i < ship.length; ++i)
        {
            int cx = BATTLE_AREA_X +  (cursor.orientation ? cursor.j : cursor.i) * BATTLE_AREA_GRID_SIZE;
            int cy = BATTLE_AREA_Y + ((cursor.orientation ? cursor.i : cursor.j) +
                    i - ship.length / 2) * BATTLE_AREA_GRID_SIZE;
            if (!cursor.orientation)
                std::swap(cx, cy);

            graphics.fillRect(cx, cy, BATTLE_AREA_GRID_SIZE, BATTLE_AREA_GRID_SIZE);
        }
    }

    const auto text = std::string{"player #"} +
        (gameBattleArea == guiGameState->getBattleArea1().get() ? '1' : '2') + " is placing ships...";
    graphics.drawText(*font, text, virtualWidth / 2 - text.length() * font->getSize() / 4,
            BATTLE_AREA_Y + BATTLE_AREA_WIDTH + 35, {80, 0, 0, 255});

    if (showInfo)
    {
        const std::string text{"place at least one ship"};
        graphics.drawText(*font, text, virtualWidth / 2 - text.length() * font->getSize() / 4,
                BATTLE_AREA_Y + BATTLE_AREA_WIDTH + 70, {0, 0, 80, 255});
    }

    for (const auto& b : buttons)
        b->render(graphics);
}

void GuiGameShipsPlacingState::handle(const SDL_Event& event) noexcept
{
    if (cursor.enabled)
    {
        const int mx = event.button.x;
        const int my = event.button.y;

        if (mx >= BATTLE_AREA_X && mx < BATTLE_AREA_X + BATTLE_AREA_WIDTH &&
            my >= BATTLE_AREA_Y && my < BATTLE_AREA_Y + BATTLE_AREA_WIDTH)
        {
            const std::array<Ship, 5>& ships = guiGameState->getShips();

            const int length =
                guiGameState->getShips()[static_cast<unsigned>(cursor.shipType)].length;

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_RIGHT)
                    cursor.orientation = !cursor.orientation;
                else if (event.button.button == SDL_BUTTON_LEFT)
                {
                    guiGameState->placeShip(*gameBattleArea, cursor.shipType,
                            cursor.i,
                            cursor.j, cursor.orientation);
                    if (!ships[static_cast<unsigned>(cursor.shipType)].amount)
                        cursor.enabled = false;
                    updateButtons();
                }
            }

            cursor.j = (mx - BATTLE_AREA_X) * 10 / BATTLE_AREA_WIDTH;
            cursor.i = (my - BATTLE_AREA_Y) * 10 / BATTLE_AREA_WIDTH;

            int* const coord = cursor.orientation ? &cursor.i : &cursor.j;
            if (*coord < length >> 1)
                *coord = length >> 1;
            else if (*coord - length / 2 + length > 10)
                     *coord = 10 + length / 2 - length;
        }
    }

    for (const auto& b : buttons)
        b->handle(event);
}

void GuiGameShipsPlacingState::updateButtons() noexcept
{
    const std::array<Ship, 5> ships = guiGameState->getShips();

    for (unsigned i = 0; i < 5; ++i)
    {
        buttons[i]->setName(buttonNames[i] + " x" + std::to_string(ships[i].amount));
        buttons[i]->setEnabled(ships[i].amount);
    }
}

