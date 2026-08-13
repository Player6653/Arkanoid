// хранит текущее состояние
#pragma once
#include "IState.h"
#include <memory>

// Управляет тем, какой экран сейчас активен.
class StateManager {
public:
    explicit StateManager(std::unique_ptr<IState> initialState);

    void handleInput(const sf::Event& event);
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);
    bool wantsExit() const { return m_current->wantsExit(); }

private:
    std::unique_ptr<IState> m_current;
};
