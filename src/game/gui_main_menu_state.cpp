#include "gui_main_menu_state.h"
#include "gui_states.h"
#include "application.h"
#include "gui_game_state_manager.h"
#include "gui_button_factory.h"

#include "engine/gui_button.h"
#include "engine/resource_manager.h"
#include "engine/graphics.h"
#include "engine/texture_atlas.h"
#include "engine/font.h"

using battleship::engine::Graphics;
using battleship::engine::GuiButton;
using battleship::engine::Font;
using battleship::engine::ResourceManager;
using battleship::engine::TextureAtlas;

using battleship::game::GuiMainMenuState;

namespace
{
    constexpr int BUTTON_SCALE = 2;
}

GuiMainMenuState::GuiMainMenuState(Application& application) :
    GuiState{static_cast<unsigned>(GuiStates::MAIN_MENU)}
{
    ResourceManager& resourceManager = application.getResourceManager();
    fontLogo = resourceManager.get<Font>("font_64");

    const GuiButtonFactory guiButtonFactory{resourceManager};

    const Graphics& graphics = application.getGraphics();

    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight(); 

    const TextureAtlas& textureAtlasButton = guiButtonFactory.getTextureAtlas();

    const int buttonWidth  = textureAtlasButton.getArea("released_button", true)->w * BUTTON_SCALE;
    const int buttonHeight = textureAtlasButton.getArea("released_button", true)->h * BUTTON_SCALE;

    const int buttonX = (virtualWidth  - buttonWidth ) >> 1;
    const int buttonY = (virtualHeight - buttonHeight) >> 1;

    buttons.push_back(guiButtonFactory.createButton("new game",
                [&application] {
                    application.getGuiGameStateManager().
                        setCurrentState(static_cast<unsigned>(GuiStates::GAME_MODE_SELECTION));

                }, buttonX, buttonY, BUTTON_SCALE));

    buttons.push_back(guiButtonFactory.createButton("exit",
                [&application] {
                    application.stop();

                }, buttonX, buttonY + 5 + buttonHeight, BUTTON_SCALE));
}

GuiMainMenuState::GuiMainMenuState(GuiMainMenuState&&) noexcept = default;

GuiMainMenuState::~GuiMainMenuState() = default;

GuiMainMenuState& GuiMainMenuState::operator=(GuiMainMenuState&&) noexcept = default;

void GuiMainMenuState::render(const Graphics& graphics, unsigned delta) noexcept
{
    const int virtualWidth  = graphics.getVirtualWidth();
    const int virtualHeight = graphics.getVirtualHeight();

    graphics.setColor(150, 150, 150);
    graphics.fillRect(0, 0, virtualWidth, virtualHeight);

    graphics.setColor(0, 0, 30);
    graphics.fillRect(virtualWidth / 2 - 180, 60, 380, 280);

    graphics.setColor(100, 100, 100);
    graphics.fillRect(virtualWidth / 2 - 190, 50, 380, 280);

    graphics.setColor(80, 80, 80);
    graphics.fillRect(virtualWidth / 2 - 190, virtualHeight / 2 - 25, 380, 90);

    graphics.drawText(*fontLogo, "Battleship", 140, 100, {80, 0, 0, 255});

    for (const auto& b : buttons)
        b->render(graphics);
}

void GuiMainMenuState::handle(const SDL_Event& event) noexcept
{
    for (const auto& b : buttons)
        b->handle(event);
}

