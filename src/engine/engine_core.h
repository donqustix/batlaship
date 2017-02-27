#ifndef ENGINECORE_H
#define ENGINECORE_H

namespace battleship
{
    namespace engine
    {
        struct EngineCore
        {
            struct SDL
            {
                SDL();
                ~SDL();
            } sdl;

            struct SDL_ttf
            {
                SDL_ttf();
                ~SDL_ttf();
            } ttf;

            struct SDL_image
            {
                SDL_image();
                ~SDL_image();
            } image;

            EngineCore() = default;
            EngineCore(const EngineCore&) = delete;
            EngineCore& operator=(const EngineCore&) = delete;
        };
    }
}

#endif
