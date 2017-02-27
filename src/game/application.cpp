#include "application.h"
#include "gui_game_state_manager.h"
#include "resource_loader.h"

#include "engine/engine_core.h"
#include "engine/window.h"
#include "engine/timer.h"
#include "engine/config.h"
#include "engine/graphics.h"
#include "engine/resource_manager.h"

#include <SDL2/SDL.h>

using battleship::game::Application;

using battleship::engine::EngineCore;
using battleship::engine::Window;
using battleship::engine::Timer;
using battleship::engine::Config;
using battleship::engine::Graphics;
using battleship::engine::ResourceManager;

constexpr Uint32 SPF = 1000 / 30;

Application::Application() : engineCore{std::make_unique<EngineCore>()}
{
    resourceManager = std::make_unique<ResourceManager>();

    const auto windowConfig = Config::readFile("res/configs/window.cfg");
    window = std::make_unique<Window>("Battleship", windowConfig.findValue<int>("window_width"),
                                                    windowConfig.findValue<int>("window_height"));
    graphics = std::make_unique<Graphics>(*window, windowConfig.findValue<int>("virtual_width"),
                                                   windowConfig.findValue<int>("virtual_height"));
    ResourceLoader{*this}.load(Config::readFile("res/configs/resources.cfg"));

    guiGameStateManager = std::make_unique<GuiGameStateManager>();
    guiGameStateManager->init(*this);
}

Application::Application(Application&&) noexcept = default;

Application::~Application() = default;

Application& Application::operator=(Application&&) noexcept = default;

void Application::run() noexcept
{
    Timer timer;

    SDL_Event sdlEvent;

    while (running)
    {
        const Uint32 elapsedTime = timer.computeElapsedTime();

        guiGameStateManager->updateCurrentState();

        while (::SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_QUIT)
                running = false;

            guiGameStateManager->handle(sdlEvent);
        }

        ::SDL_RenderClear(const_cast<SDL_Renderer*>(graphics->getSdlRenderer()));
        guiGameStateManager->render(*graphics, elapsedTime);
        ::SDL_RenderPresent(const_cast<SDL_Renderer*>(graphics->getSdlRenderer()));

        if (elapsedTime < SPF)
            ::SDL_Delay(SPF - elapsedTime);
    }
}

