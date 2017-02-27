#include "game_battle_area.h"

#include "engine/graphics.h"
#include "engine/resource_manager.h"
#include "engine/texture_atlas.h"
#include "engine/config.h"

#include <utility>
#include <cmath>
#include <iostream>

using battleship::game::GameBattleArea;

using battleship::engine::Graphics;
using battleship::engine::ResourceManager;
using battleship::engine::TextureAtlas;
using battleship::engine::Config;

GameBattleArea::GameBattleArea(ResourceManager& resourceManager)
{
    TextureAtlas shipsAtlas = TextureAtlas::load(Config::readFile("res/configs/ships.cfg"), resourceManager);
    this->shipsAtlas = std::make_unique<TextureAtlas>(std::move(shipsAtlas));
}

GameBattleArea::GameBattleArea(GameBattleArea&&) noexcept = default;

GameBattleArea::~GameBattleArea() = default;

GameBattleArea& GameBattleArea::operator=(GameBattleArea&&) noexcept = default;

void GameBattleArea::render(const Graphics& graphics, unsigned delta, int x, int y, int scale,
        bool ships) const noexcept
{
    const int gridSize = GameBattleArea::Grid::calculateSize(scale);
    const int width = gridSize * 10;

    graphics.setColor(80, 80, 80);
    graphics.fillRect(x + 5, y + 5, width, width);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            const Grid* const grid = (grids + i * 10 + j);

            if (!grid->destroyed)
            {
                const int shadowBrightness = grid->minShipDistance ? 4 * grid->minShipDistance : 64;
                graphics.setColor(shadowBrightness, shadowBrightness, shadowBrightness);
                graphics.fillRect(x + j * gridSize + 5, y + i * gridSize + 5, gridSize, gridSize);

                graphics.setColor(0, 0, grid->minShipDistance ? 255 / grid->minShipDistance : 12);
                graphics.fillRect(x + j * gridSize, y + i * gridSize, gridSize, gridSize);

                if (ships && grid->ship)
                {
                    graphics.drawTexture(shipsAtlas->getTexture(), grid->textureArea,
                            {x + j * gridSize, y + i * gridSize, gridSize, gridSize});
                }
            }
        }
    }
}

void GameBattleArea::reset() noexcept
{
    for (auto& g : grids)
        g = {};
    ships = 0;
}

void GameBattleArea::setShip(const std::string& type, int i, int j) noexcept
{
    const SDL_Rect* const textureArea = shipsAtlas->getArea(type);
    if (!textureArea)
    {
        std::cout << "there is no texture for " << type << std::endl;
        return;
    }

    (grids + i * 10 + j)->textureArea = *textureArea;
    (grids + i * 10 + j)->ship = true;
    computeDistances(i, j);
    ++ships;
}

void GameBattleArea::destroyShip(int i, int j) noexcept
{
    (grids + i * 10 + j)->destroyed = true;
    if ((grids + i * 10 + j)->ship)
    {
        (grids + i * 10 + j)->ship = false;
        for (unsigned k = 0; k < 100; ++k)
            (grids + k)->minShipDistance = 0;
        for (int n = 0; n < 10; ++n)
            for (int m = 0; m < 10; ++m)
                if ((grids + n * 10 + m)->ship)
                    computeDistances(n, m);
        --ships;
    }
}

void GameBattleArea::computeDistances(int i, int j) noexcept
{
    for (int n = 0; n < 10; ++n)
    {
        for (int m = 0; m < 10; ++m)
        {
            Grid* const grid = (grids + n * 10 + m);
            if (grid->ship)
                continue;

            const int distance = std::abs(i - n) + std::abs(j - m);
            if (!grid->minShipDistance)
                 grid->minShipDistance = distance;
            else if (grid->minShipDistance > distance)
                     grid->minShipDistance = distance;
        }
    }
    (grids + i * 10 + j)->minShipDistance = 1;
}

