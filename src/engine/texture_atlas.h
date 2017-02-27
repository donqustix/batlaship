#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include <SDL2/SDL_rect.h>

#include <map>
#include <string>

namespace battleship
{
    namespace engine
    {
        class Config;
        class Texture;
        class ResourceManager;

        class TextureAtlas
        {
            std::map<std::string, SDL_Rect> coordinates;
            const Texture* texture;

        public:
            static TextureAtlas load(const Config& config, ResourceManager& resourceManager);

            TextureAtlas(std::map<std::string, SDL_Rect> coordinates, const Texture& texture) noexcept;

            const SDL_Rect* getArea(const std::string& name, bool ex = false) const;

            const Texture& getTexture() const noexcept {return *texture;}
        };
    }
}

#endif
