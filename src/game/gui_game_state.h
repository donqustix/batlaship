#ifndef GUIGAMESTATE_H
#define GUIGAMESTATE_H

#include "ship.h"

#include "engine/gui_state.h"

#include <memory>
#include <array>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class Font;
        class GuiButton;
    }

    namespace game
    {
        class GameBattleArea;
        class GuiGameStateManager;
        class Application;
        class GamePlayer;

        class GuiGameState : public engine::GuiState
        {
            std::unique_ptr<GamePlayer> player1;
            std::unique_ptr<GamePlayer> player2;

            std::unique_ptr<GameBattleArea> battleArea1;
            std::unique_ptr<GameBattleArea> battleArea2;

            std::unique_ptr<engine::GuiButton> buttonMainMenu;
            const engine::Font* font;

            std::array<Ship, 5> ships{{{   ShipTypes::CARRIER, 1, 5},
                                       {ShipTypes::BATTLESHIP, 1, 4},
                                       {   ShipTypes::CRUISER, 1, 3},
                                       { ShipTypes::SUBMARINE, 2, 2},
                                       { ShipTypes::DESTROYER, 2, 1}}};

            GuiGameStateManager* guiGameStateManager;

            enum class States
            {
                PLAYER1_PLACES_SHIPS,
                PLAYER2_PLACES_SHIPS,
                PLAYER1_ATTACKS,
                PLAYER2_ATTACKS

            } currentState = States::PLAYER1_PLACES_SHIPS;

            struct
            {
                int i = -1;
                int j;
                bool player;
            } cursorAttack;

            bool isGameOver() const noexcept;

        public:
            explicit GuiGameState(Application& application);
            GuiGameState(GuiGameState&&) noexcept;
            ~GuiGameState();

            GuiGameState& operator=(GuiGameState&&) noexcept;

            void render(const engine::Graphics& graphics, unsigned delta) noexcept override;
            void handle(const SDL_Event& event) noexcept override;

            bool placeShip(GameBattleArea& gameBattleArea, ShipTypes shipType,
                    int i, int j, bool orientation) noexcept;
            bool attack(GameBattleArea& gameBattleArea, int i, int j) noexcept;
            void reset(GameBattleArea& gameBattleArea) noexcept;

            template<typename T, typename... Args>
            void setPlayer1(Args&&... args) {player1 = std::make_unique<T>(std::forward<Args>(args)...);}

            template<typename T, typename... Args>
            void setPlayer2(Args&&... args) {player2 = std::make_unique<T>(std::forward<Args>(args)...);}

            const auto& getBattleArea1() const noexcept {return battleArea1;}
            const auto& getBattleArea2() const noexcept {return battleArea2;}

            const auto& getShips() const noexcept {return ships;}
        };
    }
}

#endif
