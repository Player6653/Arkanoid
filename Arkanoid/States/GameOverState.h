// экран смерти
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include <SFML/Audio.hpp>
#include <string>

class GameOverState : public IState {
public:
    GameOverState(GameContext& context, int finalScore);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    int m_finalScore;
    std::string m_enteredName;
    int m_enteredNameLength = 0; // в символах
    bool m_confirmed = false;
    sf::Sound m_gameOverSound;
};
