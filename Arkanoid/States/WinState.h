// экран победы.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>

class WinState : public IState {
public:
    WinState(GameContext& context, int score);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    enum class Choice { Yes, No };

    GameContext& m_context;
    int m_score;
    StaleKeyGuard m_staleKeys;
    Choice m_selected = Choice::Yes;
    bool m_confirmed = false;

    sf::Sound m_moveSound;
    sf::Sound m_confirmSound;

    void toggleSelection();
    void confirm(Choice choice);

    // Индекс варианта (0 - Да, 1 - Нет) под курсором мыши или -1.
    int itemIndexAt(int mouseY) const;
};
