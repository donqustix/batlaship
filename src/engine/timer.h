#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL_stdinc.h>

namespace battleship
{
    namespace engine
    {
        class Timer
        {
            Uint32 previousTime;

        public:
            Timer() noexcept;

            Uint32 computeElapsedTime() noexcept;
        };
    }
}

#endif
