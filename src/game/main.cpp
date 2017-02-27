#include "application.h"

#include <iostream>

int main()
{
    try
    {
        battleship::game::Application application;
        application.run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
