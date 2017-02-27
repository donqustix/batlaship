#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_video.h>

#include <string>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class Window
        {
            SDL_Window* sdlWindow;
            int width;
            int height;

        public:
            friend void swap(Window& window1, Window& window2) noexcept
            {
                using std::swap;

                swap(window1.sdlWindow, window2.sdlWindow);
                swap(window1.width,     window2.width);
                swap(window1.height,    window2.height);
            }

            Window(const std::string& title, int width, int height,
                    int x = SDL_WINDOWPOS_CENTERED,
                    int y = SDL_WINDOWPOS_CENTERED, Uint32 flags = SDL_WINDOW_SHOWN);

            Window(Window&& window) noexcept : sdlWindow{window.sdlWindow}, width{window.width},
                height{window.height}
            {
                window.sdlWindow = nullptr;
                window.width = window.height = 0;
            }

            ~Window();

            Window& operator=(Window&& window) noexcept
            {
                if (this == &window) return *this;
                swap(*this, window);
                return *this;
            }

            const auto* getSdlWindow() const noexcept {return sdlWindow;}
            auto getWidth() const noexcept {return width;}
            auto getHeight() const noexcept {return height;}
        };
    }
}

#endif
