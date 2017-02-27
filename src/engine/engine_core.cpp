#include "engine_core.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>

using battleship::engine::EngineCore;

EngineCore::SDL::SDL()
{
    if (::SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error{::SDL_GetError()};
}

EngineCore::SDL::~SDL()
{
    ::SDL_Quit();
}

EngineCore::SDL_ttf::SDL_ttf()
{
    if (::TTF_Init())
        throw std::runtime_error{::SDL_GetError()};
}

EngineCore::SDL_ttf::~SDL_ttf()
{
    ::TTF_Quit();
}

EngineCore::SDL_image::SDL_image()
{
    if (!::IMG_Init(IMG_InitFlags::IMG_INIT_PNG))
        throw std::runtime_error{::SDL_GetError()};
}

EngineCore::SDL_image::~SDL_image()
{
    ::IMG_Quit();
}

