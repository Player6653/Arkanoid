#include "StateManager.h"

StateManager::StateManager(std::unique_ptr<IState> initialState)
    : m_current(std::move(initialState))
{
}

void StateManager::handleInput(const sf::Event& event)
{
    // событие, которое сейчас активно.
    m_current->handleInput(event);
}

void StateManager::draw(sf::RenderWindow& window)
{
    // отрисовка.
    m_current->draw(window);
}

void StateManager::update(sf::Time dt, const sf::RenderWindow& window)
{
    m_current->update(dt, window);

    std::unique_ptr<IState> next = m_current->nextState();

    if (next) {
        m_current = std::move(next);
    }
}
