#ifndef TEXTURE_H
#define TEXTURE_H

#include "resource.h"

#include <SDL2/SDL_render.h>

#include <string>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class Texture : public Resource
        {
            SDL_Texture* sdlTexture;
            int width;
            int height;

        public:
            friend void swap(Texture& texture1, Texture& texture2) noexcept
            {
                using std::swap;

                swap(texture1.sdlTexture, texture2.sdlTexture);
                swap(texture1.width,      texture2.width);
                swap(texture1.height,     texture2.height);
            }

            Texture(SDL_Renderer* sdlRenderer, const std::string& filepath);

            Texture(Texture&& texture) noexcept : sdlTexture{texture.sdlTexture}, width{texture.width},
                height{texture.height}
            {
                texture.sdlTexture = nullptr;
                texture.width = texture.height = 0;
            }

            ~Texture();

            Texture& operator=(Texture&& texture) noexcept
            {
                if (this == &texture) return *this;
                swap(*this, texture);
                return *this;
            }

            const auto* getSdlTexture() const noexcept {return sdlTexture;}
            auto getWidth() const noexcept {return width;}
            auto getHeight() const noexcept {return height;}
        };
    }
}

#endif
