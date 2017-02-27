#include "graphics.h"
#include "window.h"
#include "texture.h"
#include "font.h"

#include <stdexcept>

using battleship::engine::Graphics;

Graphics::Graphics(const Window& window, int virtualWidth, int virtualHeight) :
    sdlRenderer{[&window]{
        SDL_Renderer* const sdlRenderer = ::SDL_CreateRenderer(const_cast<SDL_Window*>(window.getSdlWindow()),
                    -1, SDL_RENDERER_SOFTWARE);
        if (!sdlRenderer)
            throw std::runtime_error{::SDL_GetError()};

        return sdlRenderer;

    }(), ::SDL_DestroyRenderer}, virtualWidth{virtualWidth}, virtualHeight{virtualHeight}
{
    if (::SDL_RenderSetLogicalSize(sdlRenderer.get(), virtualWidth, virtualHeight) < 0)
        throw std::runtime_error{::SDL_GetError()};
}

Graphics::Graphics(Graphics&&) noexcept = default;

Graphics::~Graphics() = default;

Graphics& Graphics::operator=(Graphics&&) noexcept = default;

void Graphics::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const noexcept
{
    ::SDL_SetRenderDrawColor(sdlRenderer.get(), r, g, b, a);
}

void Graphics::fillRect(int x, int y, int w, int h) const noexcept
{
    const SDL_Rect rect{x, y, w, h};
    ::SDL_RenderFillRect(sdlRenderer.get(), &rect);
}

void Graphics::drawRect(int x, int y, int w, int h) const noexcept
{
    const SDL_Rect rect{x, y, w, h};
    ::SDL_RenderDrawRect(sdlRenderer.get(), &rect);
}

void Graphics::drawText(const Font& font, const std::string& text, int x, int y, SDL_Color color) const noexcept
{
    SDL_Surface* const sdlSurface = ::TTF_RenderText_Solid(const_cast<TTF_Font*>(font.getTtfFont()),
            text.c_str(), color);
    SDL_Texture* const sdlTexture = ::SDL_CreateTextureFromSurface(sdlRenderer.get(), sdlSurface);
    ::SDL_FreeSurface(sdlSurface);

    const SDL_Rect dstrect{x, y, sdlSurface->w, sdlSurface->h};
    ::SDL_RenderCopy(sdlRenderer.get(), sdlTexture, nullptr, &dstrect);
    ::SDL_DestroyTexture(sdlTexture);
}

void Graphics::drawTexture(const Texture& texture, SDL_Rect srcrect, SDL_Rect dstrect) const noexcept
{
    ::SDL_RenderCopy(sdlRenderer.get(), const_cast<SDL_Texture*>(texture.getSdlTexture()), &srcrect, &dstrect);
}

void Graphics::drawTexture(const Texture& texture, SDL_Rect dstrect) const noexcept
{
    ::SDL_RenderCopy(sdlRenderer.get(), const_cast<SDL_Texture*>(texture.getSdlTexture()), nullptr, &dstrect);
}

