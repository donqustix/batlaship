#include "texture.h"

#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <memory>

using battleship::engine::Texture;

Texture::Texture(SDL_Renderer* sdlRenderer, const std::string& filepath)
{
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> sdlSurface{
        [&filepath]
        {
            SDL_Surface* const sdlSurface = ::IMG_Load(filepath.c_str());
            if (!sdlSurface)
                throw std::runtime_error{::SDL_GetError()};

            return sdlSurface;

        }(), ::SDL_FreeSurface};

    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> sdlTexture{
        [sdlRenderer, &sdlSurface]
        {
            SDL_Texture* const sdlTexture = ::SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface.get());
            if (!sdlTexture)
                throw std::runtime_error{::SDL_GetError()};

            return sdlTexture;

        }(), ::SDL_DestroyTexture
    };

    if (::SDL_QueryTexture(sdlTexture.get(), nullptr, nullptr, &width, &height))
        throw std::runtime_error{::SDL_GetError()};

    this->sdlTexture = sdlTexture.release();
}

Texture::~Texture()
{
    ::SDL_DestroyTexture(sdlTexture);
}

