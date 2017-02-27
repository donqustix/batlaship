#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>

namespace battleship
{
    namespace engine
    {
        namespace utility
        {
            template<typename T>
            T convert(const std::string& str) noexcept
            {
                std::istringstream stream{str};
                T value;
                stream >> value;
                return value;
            }
        }
    }
}

#endif
