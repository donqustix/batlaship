#include "timer.h"

#include <SDL2/SDL_timer.h>

using battleship::engine::Timer;

Timer::Timer() noexcept : previousTime{::SDL_GetTicks()} {}

Uint32 Timer::computeElapsedTime() noexcept
{
    const Uint32 currentTime = ::SDL_GetTicks();
    const Uint32 elapsedTime = currentTime - previousTime;
    previousTime = currentTime;

    return elapsedTime;
}

