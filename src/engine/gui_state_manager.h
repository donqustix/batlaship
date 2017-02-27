#ifndef GUISTATEMANAGER_H
#define GUISTATEMANAGER_H

#include "gui_state.h"

#include <SDL2/SDL_events.h>

#include <vector>
#include <memory>
#include <utility>

namespace battleship
{
    namespace engine
    {
        class GuiState;
        class Graphics;

        class GuiStateManager
        {
            std::vector<std::unique_ptr<GuiState>> states;
            unsigned currentState = 0;

            GuiState* activeState = nullptr;

        public:
            GuiStateManager() noexcept;
            GuiStateManager(GuiStateManager&&) noexcept;
            ~GuiStateManager();

            GuiStateManager& operator=(GuiStateManager&&) noexcept;

            template<typename T, typename... Args>
            void addState(Args&&... args) noexcept
            {
                states.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            }

            template<typename T>
            T* findState(unsigned state) noexcept
            {
                for (auto& s : states)
                {
                    if (s->getState() == state)
                        return dynamic_cast<T*>(s.get());
                }

                return nullptr;
            }

            void updateCurrentState() noexcept;
            void render(const Graphics& graphics, unsigned delta) const noexcept;
            void handle(const SDL_Event& event) const noexcept;

            void setCurrentState(unsigned currentState) noexcept {this->currentState = currentState;}
            auto getCurrentState() const noexcept {return currentState;}
        };
    }
}

#endif
