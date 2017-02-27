#include "gui_game_ships_attacking_state.h"
#include "gui_states.h"
#include "gui_game_state_manager.h"
#include "gui_game_state.h"
#include "application.h"
#include "game_battle_area.h"
#include "gui_button_factory.h"

#include "engine/graphics.h"
#include "engine/gui_button.h"
#include "engine/texture_atlas.h"
#include "engine/resource_manager.h"
#include "engine/font.h"

using battleship::game::GuiGameShipsAttackingState;

using battleship::engine::Graphics;
using battleship::engine::GuiButton;
using battleship::engine::ResourceManager;
using battleship::engine::TextureAtlas;
using battleship::engine::Font;

namespace
{
    constexpr int BUTTON_SCALE = 2;
    constexpr int BATTLE_AREA_SCALE = 14;
    constexpr int BATTLE_AREA_GRID_SIZE =
        battleship::game::GameBattleArea::Grid::calculateSize(BATTLE_AREA_SCALE);
    constexpr int BATTLE_AREA_WIDTH = BATTLE_AREA_GRID_SIZE * 10;

    constexpr int BATTLE_AREA_Y = 15;   
    int BATTLE_AREA_X;
}

GuiGameShipsAttackingState::GuiGameShipsAttackingState(Application& application) :
    GuiState{static_cast<unsigned>(GuiStates::GAME_SHIPS_ATTACKING)}
{
    guiGameState = application.getGuiGameStateManager().
        findState<GuiGameState>(static_cast<unsigned>(GuiStates::GAME_STATE));

    const Graphics& graphics = application.getGraphics();

    const int virtualWidth = graphics.getVirtualWidth();

    BATTLE_AREA_X = (virtualWidth - BATTLE_AREA_WIDTH) >> 1;

    ResourceManager& resourceManager = application.getResourceManager();
    font = resourceManager.get<Font>("font_24");

    const GuiButtonFactory guiButtonFactory{resourceManager};

    const TextureAtlas& textureAtlasButton = guiButtonFactory.getTextureAtlas();

    const int buttonWidth = textureAtlasButton.getArea("pressed_button", true)->w * BUTTON_SCALE;

    buttonFire = guiButtonFactory.createButton("FIRE!",
            [&application, this] {
                guiGameState->attack(*gameBattleArea, cursor.i, cursor.j);
                for (int i = 0; i < 10; ++i)
                {
                    for (int j = 0; j < 10; ++j)
                    {
                        if (!gameBattleArea->getGrid(i, j).destroyed)
                        {
                            cursor.i = i;
                            cursor.j = j;
                            break;
                        }
                    }
                }
                application.getGuiGameStateManager().
                    setCurrentState(static_cast<unsigned>(GuiStates::GAME_STATE));
            },  BATTLE_AREA_X + (BATTLE_AREA_WIDTH - buttonWidth) / 2,
                BATTLE_AREA_Y +  BATTLE_AREA_WIDTH + 15, BUTTON_SCALE);
}

GuiGameShipsAttackingState::GuiGameShipsAttackingState(GuiGameShipsAttackingState&&) noexcept = default;

GuiGameShipsAttackingState::~GuiGameShipsAttackingState() = default;

GuiGameShipsAttackingState& GuiGameShipsAttackingState::operator=(
        GuiGameShipsAttackingState&&) noexcept = default;

void GuiGameShipsAttackingState::render(const Graphics& graphics, unsigned delta) noexcept
{
    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    graphics.setColor(150, 150, 150);
    graphics.fillRect(0, 0, virtualWidth, virtualHeight);

    gameBattleArea->render(graphics, delta, BATTLE_AREA_X, BATTLE_AREA_Y, BATTLE_AREA_SCALE, false);

    static float time = 0.0F;
    if (time >= 10.0F)
        time = 0.0F;
    time += 0.01F * delta;

    graphics.setColor(time * 25.5F, 0, 0);
    graphics.fillRect(BATTLE_AREA_X + BATTLE_AREA_GRID_SIZE * cursor.j,
                      BATTLE_AREA_Y + BATTLE_AREA_GRID_SIZE * cursor.i,
                                      BATTLE_AREA_GRID_SIZE, BATTLE_AREA_GRID_SIZE);
    buttonFire->render(graphics);

    const auto text = std::string{"player #"} +
        (gameBattleArea == guiGameState->getBattleArea2().get() ? '1' : '2') + " is moving...";
    graphics.drawText(*font, text, virtualWidth / 2 - text.length() * font->getSize() / 4,
            BATTLE_AREA_Y + BATTLE_AREA_WIDTH + 50, {80, 0, 0, 255});
}

void GuiGameShipsAttackingState::handle(const SDL_Event& event) noexcept
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        const int mx = event.button.x;
        const int my = event.button.y;

        if (mx >= BATTLE_AREA_X && mx < BATTLE_AREA_X + BATTLE_AREA_WIDTH &&
            my >= BATTLE_AREA_Y && my < BATTLE_AREA_Y + BATTLE_AREA_WIDTH)
        {
            const int nj = (mx - BATTLE_AREA_X) * 10 / BATTLE_AREA_WIDTH;
            const int ni = (my - BATTLE_AREA_Y) * 10 / BATTLE_AREA_WIDTH;

            if (!gameBattleArea->getGrid(ni, nj).destroyed)
            {
                cursor.j = nj;
                cursor.i = ni;
            }
        }
    }
    buttonFire->handle(event);
}

