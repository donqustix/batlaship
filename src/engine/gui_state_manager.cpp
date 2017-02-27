#include "gui_state_manager.h"

using battleship::engine::GuiStateManager;

GuiStateManager::GuiStateManager() noexcept = default;

GuiStateManager::GuiStateManager(GuiStateManager&&) noexcept = default;

GuiStateManager::~GuiStateManager() = default;

GuiStateManager& GuiStateManager::operator=(GuiStateManager&&) noexcept = default;

void GuiStateManager::updateCurrentState() noexcept
{
    if (!activeState || activeState->getState() != currentState)
    {
        for (auto& s : states)
        {
            if (s->getState() == currentState)
            {
                activeState = s.get();
                break;
            }
        }
    }
}

void GuiStateManager::render(const Graphics& graphics, unsigned delta) const noexcept
{
    activeState->render(graphics, delta);
}

void GuiStateManager::handle(const SDL_Event& event) const noexcept
{
    activeState->handle(event);
}

