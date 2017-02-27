#include "gui_game_mode_selection_state.h"
#include "gui_states.h"
#include "application.h"
#include "gui_game_state_manager.h"
#include "gui_game_state.h"
#include "game_brain_player.h"
#include "game_cpu_player.h"
#include "gui_button_factory.h"

#include "engine/resource_manager.h"
#include "engine/font.h"
#include "engine/graphics.h"
#include "engine/gui_button.h"
#include "engine/texture_atlas.h"

using battleship::game::GuiGameModeSelectionState;

using battleship::engine::Graphics;
using battleship::engine::Font;
using battleship::engine::TextureAtlas;
using battleship::engine::ResourceManager;
using battleship::engine::GuiButton;

namespace
{
    constexpr int BUTTON_SCALE = 2;
}

GuiGameModeSelectionState::GuiGameModeSelectionState(Application& application) :
    GuiState{static_cast<unsigned>(GuiStates::GAME_MODE_SELECTION)}
{
    font = application.getResourceManager().get<Font>("font_24");

    ResourceManager& resourceManager = application.getResourceManager();

    const GuiButtonFactory guiButtonFactory{resourceManager};

    const Graphics& graphics = application.getGraphics();

    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight(); 

    const TextureAtlas& textureAtlasButton = guiButtonFactory.getTextureAtlas();

    const int buttonWidth  = textureAtlasButton.getArea("released_button", true)->w * BUTTON_SCALE;
    const int buttonHeight = textureAtlasButton.getArea("released_button", true)->h * BUTTON_SCALE;

    const int buttonX = (virtualWidth  - buttonWidth ) >> 1;
    const int buttonY = (virtualHeight - buttonHeight) >> 1;

    buttons.push_back(guiButtonFactory.createButton("cpu vs cpu",
                [&application] {
                    const auto guiGameState =
                        application.getGuiGameStateManager().findState<GuiGameState>(
                                static_cast<unsigned>(GuiStates::GAME_STATE));
                    guiGameState->setPlayer1<GameCpuPlayer>();
                    guiGameState->setPlayer2<GameCpuPlayer>();
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::GAME_STATE));

                }, buttonX, buttonY, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("brain vs cpu",
                [&application] {
                    const auto guiGameState =
                        application.getGuiGameStateManager().findState<GuiGameState>(
                                static_cast<unsigned>(GuiStates::GAME_STATE));
                    guiGameState->setPlayer1<GameBrainPlayer>(application.getGuiGameStateManager());
                    guiGameState->setPlayer2<GameCpuPlayer>();
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::GAME_STATE));

                }, buttonX, buttonY + 5 + buttonHeight, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("brain vs brain",
                [&application] {
                    const auto guiGameState =
                        application.getGuiGameStateManager().findState<GuiGameState>(
                                static_cast<unsigned>(GuiStates::GAME_STATE));
                    guiGameState->setPlayer1<GameBrainPlayer>(application.getGuiGameStateManager());
                    guiGameState->setPlayer2<GameBrainPlayer>(application.getGuiGameStateManager());
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::GAME_STATE));

                }, buttonX, buttonY + 10 + buttonHeight * 2, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("back",
                [&application] {
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::MAIN_MENU));

                }, buttonX, buttonY + 15 + buttonHeight * 3, BUTTON_SCALE));
}

GuiGameModeSelectionState::GuiGameModeSelectionState(GuiGameModeSelectionState&&) noexcept = default;

GuiGameModeSelectionState::~GuiGameModeSelectionState() = default;

GuiGameModeSelectionState& GuiGameModeSelectionState::operator=(GuiGameModeSelectionState&&) noexcept = default;

void GuiGameModeSelectionState::render(const Graphics& graphics, unsigned delta) noexcept
{
    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    const std::string selectModeText{"select the mode of the game"};

    graphics.setColor(150, 150, 150);
    graphics.fillRect(0, 0, virtualWidth, virtualHeight);

    graphics.setColor(0, 0, 30);
    graphics.fillRect(virtualWidth / 2 - 180, 60, 380, 354);

    graphics.setColor(100, 100, 100);
    graphics.fillRect(virtualWidth / 2 - 190, 50, 380, 354);

    graphics.setColor(80, 80, 80);
    graphics.fillRect(virtualWidth / 2 - 190, virtualHeight / 2 - 25, 380, 164);

    graphics.drawText(*font, selectModeText,
            virtualWidth / 2 - selectModeText.length() * font->getSize() / 4 - 10, 120, {80, 0, 0, 255});

    for (const auto& b : buttons)
        b->render(graphics);
}

void GuiGameModeSelectionState::handle(const SDL_Event& event) noexcept
{
    for (const auto& b : buttons)
        b->handle(event);
}

