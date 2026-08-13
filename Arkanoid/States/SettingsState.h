// States/SettingsState.h — экран настроек (звук/музыка)
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>

class SettingsState : public IState {
public:
    explicit SettingsState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    StaleKeyGuard m_staleKeys;
    bool m_backRequested = false;
    sf::Sound m_toggleSound; 
};
