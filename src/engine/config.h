#ifndef CONFIG_H
#define CONFIG_H

#include "utility.h"

#include <map>
#include <algorithm>
#include <string>
#include <istream>
#include <sstream>

namespace battleship
{
    namespace engine
    {
    class Config
    {
    public:
        struct Block
        {
            std::map<std::string, std::string> data;
            std::map<std::string, Block> blocks;
        };

    private:
        Block mainBlock;

        template<typename Char, typename Traits>
        void load(std::basic_istream<Char, Traits>& stream, Block& block) noexcept
        {
            std::string line;
            while (std::getline(stream, line))
            {
                line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                switch (line[0])
                {
                    case '!':
                    {
                        auto newBlockName = line.substr(1, line.length() - 1);
                        Block newBlock;
                        load(stream, newBlock);
                        block.blocks.emplace(std::move(newBlockName), std::move(newBlock));
                        break;
                    }
                    case '/':
                        return;
                    default:
                    {
                        std::istringstream sstream{line};
                        std::string key, value;
                        std::getline(sstream, key, '=');
                        std::getline(sstream, value);
                        block.data.emplace(std::move(key), std::move(value));
                        break;
                    }
                }
            }
        }

        const Block& findBlock(const std::string& route) const noexcept;

    public:
        static Config readFile(const std::string& filepath);

        template<typename Char, typename Traits>
        explicit Config(std::basic_istream<Char, Traits>& stream) noexcept
        {
            load(stream, mainBlock);
        }

        template<typename T>
        T findValue(const std::string& key, const std::string& route) const noexcept
        {
            return utility::convert<T>(findBlock(route).data.find(key)->second);
        }

        template<typename T>
        T findValue(const std::string& key) const noexcept
        {
            return utility::convert<T>(findBlock("").data.find(key)->second);
        }

        const auto& getMainBlock() const noexcept {return mainBlock;}
    };

    template<>
    inline std::string Config::findValue(const std::string& key, const std::string& route) const noexcept
    {
        return findBlock(route).data.find(key)->second;
    }

    template<>
    inline std::string Config::findValue(const std::string& key) const noexcept
    {
        return findBlock("").data.find(key)->second;
    }
    }
}

#endif
