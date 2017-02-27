#include "texture_atlas.h"
#include "config.h"
#include "resource_manager.h"
#include "texture.h"

#include <stdexcept>

using battleship::engine::TextureAtlas;

TextureAtlas TextureAtlas::load(const Config& config, ResourceManager& resourceManager)
{
    const auto texture = resourceManager.get<Texture>(config.findValue<std::string>("texture"));
    std::map<std::string, SDL_Rect> coordinates;

    for (const auto& b : config.getMainBlock().blocks)
    {
        coordinates.emplace(b.first, SDL_Rect{config.findValue<int>("x", b.first),
                                              config.findValue<int>("y", b.first),
                                              config.findValue<int>("w", b.first),
                                              config.findValue<int>("h", b.first)});
    }

    return {std::move(coordinates), *texture};
}

TextureAtlas::TextureAtlas(std::map<std::string, SDL_Rect> coordinates, const Texture& texture) noexcept :
    coordinates{std::move(coordinates)}, texture{&texture} {}

const SDL_Rect* TextureAtlas::getArea(const std::string& name, bool ex) const
{
    auto iter = coordinates.find(name);
    if (iter == coordinates.cend())
    {
        if (ex)
            throw std::runtime_error{"there is no atlas area " + name};

        return nullptr;
    }

    return &iter->second;
}

