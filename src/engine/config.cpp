#include "config.h"

#include <fstream>
#include <stdexcept>

using battleship::engine::Config;

Config Config::readFile(const std::string& filepath)
{
    std::ifstream stream{filepath, std::ios::in};
    if (!stream)
        throw std::runtime_error{"file reading error"};

    return Config{stream};
}

const Config::Block& Config::findBlock(const std::string& route) const noexcept
{
    if (route.empty()) return mainBlock;

    std::istringstream sstream{route};
    std::string blockName;

    std::getline(sstream, blockName, ':');

    auto iter = mainBlock.blocks.find(blockName);
    for (; std::getline(sstream, blockName, ':'); iter = iter->second.blocks.find(blockName));

    return iter->second;
}


