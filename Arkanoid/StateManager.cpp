#include "StateManager.h"

StateManager::StateManager(std::unique_ptr<IState> initialState)
    : m_current(std::move(initialState))
{
}

void StateManager::handleInput(const sf::Event& event)
{
    // Просто отдаём событие тому состоянию, которое сейчас активно.
    m_current->handleInput(event);
}

void StateManager::draw(sf::RenderWindow& window)
{
    // просто делегируем отрисовку текущему состоянию.
    m_current->draw(window);
}

void StateManager::update(sf::Time dt)
{
    m_current->update(dt);

    std::unique_ptr<IState> next = m_current->nextState();

    if (next) {
        m_current = std::move(next);
    }
}
