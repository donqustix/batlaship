#include "font.h"

#include <stdexcept>

using battleship::engine::Font;

Font::Font(const std::string& filepath, int size) :
    ttfFont{::TTF_OpenFont(filepath.c_str(), size)}, size{size}
{
    if (!ttfFont)
        throw std::runtime_error{::SDL_GetError()};
}

Font::~Font()
{
    ::TTF_CloseFont(ttfFont);
}

