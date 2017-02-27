#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL_render.h>

#include <memory>
#include <string>

namespace battleship
{
    namespace engine
    {
        class Window;
        class Texture;
        class Font;

        class Graphics
        {
            std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdlRenderer;
            int virtualWidth;
            int virtualHeight;

        public:
            Graphics(const Window& window, int virtualWidth, int virtualHeight);
            Graphics(Graphics&&) noexcept;
            ~Graphics();

            Graphics& operator=(Graphics&&) noexcept;

            void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const noexcept;
            void fillRect(int x, int y, int w, int h) const noexcept;
            void drawRect(int x, int y, int w, int h) const noexcept;
            void drawText(const Font& font,
                    const std::string& text, int x, int y,
                        SDL_Color color = {255, 255, 255, 255}) const noexcept;
            void drawTexture(const Texture& texture, SDL_Rect srcrect, SDL_Rect dstrect) const noexcept;
            void drawTexture(const Texture& texture, SDL_Rect dstrect) const noexcept;

            const auto* getSdlRenderer() const noexcept {return sdlRenderer.get();}
            auto getVirtualWidth() const noexcept {return virtualWidth;}
            auto getVirtualHeight() const noexcept {return virtualHeight;}
        };
    }
}

#endif
