#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

namespace battleship
{
    namespace game
    {
        class GuiGameState;
        class GameBattleArea;

        class GamePlayer
        {
        public:
            enum class Types
            {
                BRAIN, CPU
            };

        private:
            Types type;

        public:
            explicit GamePlayer(Types type) noexcept : type{type} {}
            virtual ~GamePlayer() = default;

            virtual void placeShips(GuiGameState& guiGameState,
                    GameBattleArea& gameBattleArea) const noexcept = 0;
            virtual void attack(GuiGameState& guiGameState, GameBattleArea& gameBattleArea) const noexcept = 0;

            auto getType() const noexcept {return type;}
        };
    }
}

#endif
