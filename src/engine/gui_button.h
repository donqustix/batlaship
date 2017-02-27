#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <SDL2/SDL_events.h>

#include <string>
#include <functional>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class Graphics;
        class Texture;
        class TextureAtlas;
        class Font;

        class GuiButton
        {
            enum class States
            {
                PRESSED, RELEASED, SELECTED
            } state = States::RELEASED;

            std::string name;
            std::function<void()> action;
            const Texture* texture;
            const Font* font;
            int x;
            int y;
            int scale;
            bool activation = false;
            bool enabled = true;

            SDL_Rect pressedRect;
            SDL_Rect releasedRect;
            SDL_Rect selectedRect;

        public:
            GuiButton(std::string name, std::function<void()> action, const TextureAtlas& textureAtlas,
                    const Font* font, int x, int y, int scale);

            void render(const Graphics& graphics) noexcept;
            void handle(const SDL_Event& event) noexcept;

            void setEnabled(bool enabled) noexcept {this->enabled = enabled;}

            auto getWidth()  const noexcept {return releasedRect.w * scale;}
            auto getHeight() const noexcept {return releasedRect.h * scale;}

            const auto& getName() const noexcept {return name;}

            template<typename T>
            void setName(T&& name) noexcept {this->name = std::forward<T>(name);}

            auto getX() const noexcept {return x;}
            auto getY() const noexcept {return y;}
        };
    }
}

#endif
