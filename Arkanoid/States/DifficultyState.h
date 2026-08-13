// сложность
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>

// Выбор уровня сложности
class DifficultyState : public IState {
public:
    explicit DifficultyState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    StaleKeyGuard m_staleKeys;
    bool m_backRequested = false;
    sf::Sound m_changeSound;
};
