#include "gui_button_factory.h"

#include "engine/resource_manager.h"
#include "engine/texture_atlas.h"
#include "engine/config.h"
#include "engine/gui_button.h"
#include "engine/font.h"

using battleship::game::GuiButtonFactory;

using battleship::engine::TextureAtlas;
using battleship::engine::Font;
using battleship::engine::Config;
using battleship::engine::ResourceManager;
using battleship::engine::GuiButton;

GuiButtonFactory::GuiButtonFactory(ResourceManager& resourceManager)
{
    const auto guiConfig = Config::readFile("res/configs/gui.cfg");

    TextureAtlas textureAtlas =
        TextureAtlas::load(Config::readFile(guiConfig.findValue<std::string>("button_texture_atlas")),
                resourceManager);
    this->textureAtlas = std::make_unique<TextureAtlas>(textureAtlas);

    font = resourceManager.get<Font>("font_" + guiConfig.findValue<std::string>("button_font_size"));
}

std::unique_ptr<GuiButton> GuiButtonFactory::createButton(std::string name,
        std::function<void()> action, int x, int y, int scale) const
{
    return std::make_unique<GuiButton>(std::move(name), std::move(action), *textureAtlas, font, x, y, scale);
}

