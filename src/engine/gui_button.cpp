#include "gui_button.h"
#include "graphics.h"
#include "texture_atlas.h"
#include "font.h"

using battleship::engine::GuiButton;

GuiButton::GuiButton(std::string name, std::function<void()> action, const TextureAtlas& textureAtlas,
        const Font* font, int x, int y, int scale) :
    name{std::move(name)}, action{std::move(action)}, texture{&textureAtlas.getTexture()}, font{font},
    x{x}, y{y}, scale{scale}
{
    pressedRect  = *textureAtlas.getArea("pressed_button");
    releasedRect = *textureAtlas.getArea("released_button");
    selectedRect = *textureAtlas.getArea("selected_button");
}

void GuiButton::render(const Graphics& graphics) noexcept
{
    if (!enabled)
        state = States::PRESSED;
    else if (state == States::PRESSED && !activation)
        state = States::RELEASED;

    const int width  = getWidth();
    const int height = getHeight();

    const SDL_Rect boundingBox{x, y, width, height};

    switch (state)
    {
        case States::PRESSED:
            graphics.drawTexture(*texture, pressedRect,  boundingBox);
            break;
        case States::RELEASED:
            graphics.drawTexture(*texture, releasedRect, boundingBox);
            break;
        case States::SELECTED:
            graphics.drawTexture(*texture, selectedRect, boundingBox);
            break;
    }

    graphics.drawText(*font, name,
            x + width / 2 - name.length() * font->getSize() / 4, y + (height - font->getSize()) / 2);
}

void GuiButton::handle(const SDL_Event& event) noexcept
{
    if (!enabled) return;

    const int mx = event.button.x;
    const int my = event.button.y;

    if (mx >= x && mx < x + getWidth() &&
        my >= y && my < y + getHeight())
    {
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            activation = true;
            state = States::PRESSED;
        }
        else if (activation)
        {
            action();
            activation = false;
            state = States::RELEASED;
        }
        else
            state = States::SELECTED;
    }
    else
        state = States::RELEASED;
}

