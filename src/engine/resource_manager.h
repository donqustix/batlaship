#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>

namespace battleship
{
    namespace engine
    {
        class Resource;

        class ResourceManager
        {
            std::map<std::string, std::unique_ptr<Resource>> resources;

        public:
            ResourceManager();
            ResourceManager(ResourceManager&&) noexcept;
            ~ResourceManager();

            ResourceManager& operator=(ResourceManager&& resourceManager) noexcept;

            template<typename T, typename... Args>
            T* load(const std::string& name, Args&&... args)
            {
                auto iter = resources.find(name);
                if (iter != resources.cend())
                    throw std::runtime_error{"the " + name + " resource is already exist"};

                return dynamic_cast<T*>(resources.emplace(name,
                            std::make_unique<T>(std::forward<Args>(args)...)).first->second.get());
            }

            template<typename T>
            T* get(const std::string& name)
            {
                auto iter = resources.find(name);
                if (iter == resources.cend())
                    throw std::runtime_error{"there is no " + name + " resource"};

                return dynamic_cast<T*>(iter->second.get());
            }
        };
    }
}

#endif
