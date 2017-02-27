#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

// 11:55 306

namespace battleship
{
    namespace engine
    {
        class EngineCore;
        class Window;
        class Graphics;
        class ResourceManager;
    }

    namespace game
    {
        class GuiGameStateManager;

        class Application
        {
            std::unique_ptr<engine::EngineCore> engineCore;
            std::unique_ptr<engine::Window> window;
            std::unique_ptr<engine::Graphics> graphics;
            std::unique_ptr<engine::ResourceManager> resourceManager;
            std::unique_ptr<GuiGameStateManager> guiGameStateManager;
            bool running = true;

        public:
            Application();
            Application(Application&&) noexcept;
            ~Application();

            Application& operator=(Application&&) noexcept;

            void run() noexcept;
            void stop() noexcept {running = false;}

            auto& getWindow() noexcept {return *window;}
            auto& getGraphics() noexcept {return *graphics;}
            auto& getResourceManager() noexcept {return *resourceManager;}
            auto& getGuiGameStateManager() noexcept {return *guiGameStateManager;}
        };
    }
}

#endif
