#include "resource_loader.h"
#include "application.h"

#include "engine/config.h"
#include "engine/resource_manager.h"
#include "engine/font.h"
#include "engine/texture.h"
#include "engine/graphics.h"

using battleship::game::ResourceLoader;

using battleship::engine::Config;
using battleship::engine::ResourceManager;
using battleship::engine::Font;
using battleship::engine::Texture;

void ResourceLoader::load(const Config& config) const
{
    ResourceManager& resourceManager = application->getResourceManager();

    for (const auto& pb : config.getMainBlock().blocks)
    {
        if (pb.first == "fonts")
        {
            for (const auto& pd : pb.second.data)
                resourceManager.load<Font>(pd.first, pd.second,
                        std::stoi(pd.first.substr(pd.first.find('_') + 1)));
        }
        else if (pb.first == "textures")
        {
            for (const auto& pd : pb.second.data)
                resourceManager.load<Texture>(pd.first,
                        const_cast<SDL_Renderer*>(application->getGraphics().getSdlRenderer()), pd.second);
        }
    }
}

