#ifndef GAMEBATTLEAREA_H
#define GAMEBATTLEAREA_H

#include <SDL2/SDL_rect.h>

#include <memory>

namespace battleship
{
    namespace engine
    {
        class Graphics;
        class ResourceManager;
        class TextureAtlas;
    }

    namespace game
    {
        class GuiGameState;

        class GameBattleArea
        {
            friend class GuiGameState;

        public:
            struct Grid
            {
                static constexpr auto calculateSize(int scale) noexcept {return scale << 1;}

                SDL_Rect textureArea;
                int minShipDistance;
                bool ship;
                bool destroyed;
            };

        private:
            std::unique_ptr<engine::TextureAtlas> shipsAtlas;
            Grid grids[100]{};
            unsigned ships = 0;

        private:
            void reset() noexcept;
            void setShip(const std::string& type, int i, int j) noexcept;
            void destroyShip(int i, int j) noexcept;
            void computeDistances(int i, int j) noexcept;

        public:
            explicit GameBattleArea(engine::ResourceManager& resourceManager);
            GameBattleArea(GameBattleArea&&) noexcept;
            ~GameBattleArea();

            GameBattleArea& operator=(GameBattleArea&&) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta, int x, int y, int scale,
                    bool ships) const noexcept;

            const auto& getGrid(int i, int j) const noexcept {return grids[10 * i + j];}
            auto isNoShips() const noexcept {return !ships;}
            auto getShips() const noexcept {return ships;}
        };
    }
}

#endif
