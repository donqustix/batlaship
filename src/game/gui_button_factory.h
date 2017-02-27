#ifndef GUIBUTTONFACTORY_H
#define GUIBUTTONFACTORY_H

#include <memory>
#include <string>
#include <functional>

namespace battleship
{
    namespace engine
    {
        class GuiButton;
        class TextureAtlas;
        class Font;
        class ResourceManager;
    }

    namespace game
    {
        class GuiButtonFactory
        {
            std::unique_ptr<engine::TextureAtlas> textureAtlas;
            const engine::Font* font;

        public:
            explicit GuiButtonFactory(engine::ResourceManager& resourceManager);

            std::unique_ptr<engine::GuiButton> createButton(std::string name, std::function<void()> action,
                    int x, int y, int scale) const;

            const auto& getTextureAtlas() const noexcept {return *textureAtlas;}
        };
    }
}

#endif
