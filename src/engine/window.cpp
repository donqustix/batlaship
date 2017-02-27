#include "window.h"

#include <stdexcept>

using battleship::engine::Window;

Window::Window(const std::string& title, int width, int height, int x, int y, Uint32 flags) :
    sdlWindow{::SDL_CreateWindow(title.c_str(), x, y, width, height, flags)}, width{width}, height{height}
{
    if (!sdlWindow)
        throw std::runtime_error{::SDL_GetError()};
}

Window::~Window()
{
    ::SDL_DestroyWindow(sdlWindow);
}

