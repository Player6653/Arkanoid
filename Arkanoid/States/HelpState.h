// экран помощи
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"

class HelpState : public IState {
public:
    explicit HelpState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    StaleKeyGuard m_staleKeys;
    bool m_backRequested = false;
};
