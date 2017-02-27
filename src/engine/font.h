#ifndef FONT_H
#define FONT_H

#include "resource.h"

#include <SDL2/SDL_ttf.h>

#include <string>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class Font : public Resource
        {
            TTF_Font* ttfFont;
            int size;

        public:
            friend void swap(Font& font1, Font& font2) noexcept
            {
                using std::swap;

                swap(font1.ttfFont, font2.ttfFont);
                swap(font1.size,    font2.size);
            }

            Font(const std::string& filepath, int size);

            Font(Font&& font) noexcept : ttfFont{font.ttfFont}, size{font.size}
            {
                font.ttfFont = nullptr;
                font.size = 0;
            }

            ~Font();

            Font& operator=(Font&& font) noexcept
            {
                if (this == &font) return *this;
                swap(*this, font);
                return *this;
            }

            const auto* getTtfFont() const noexcept {return ttfFont;}
            auto getSize() const noexcept {return size;}
        };
    }
}

#endif
