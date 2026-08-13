// экран таблицы рекордов
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"

// список лучших результатов и возврат в меню.
class HighscoresState : public IState {
public:
    explicit HighscoresState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    StaleKeyGuard m_staleKeys;
    bool m_backRequested = false;
};
