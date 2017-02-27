#ifndef SHIP_H
#define SHIP_H

namespace battleship
{
    namespace game
    {
        enum class ShipTypes
        {
            CARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER
        };

        struct Ship
        {
            ShipTypes type;
            unsigned amount;
            int length;
        };
    }
}

#endif
