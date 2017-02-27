#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

namespace battleship
{
    namespace engine
    {
        class Config;
    }

    namespace game
    {
        class Application;

        class ResourceLoader
        {
            Application* application;

        public:
            explicit ResourceLoader(Application& application) noexcept : application{&application} {}

            void load(const engine::Config& config) const;
        };
    }
}

#endif
