// экран победы.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>
#include <string>

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

    // Сначала игрок вводит имя для таблицы рекордов, и только потом — Да/Нет "сыграть ещё раз".
    std::string m_enteredName;
    int m_enteredNameLength = 0; // в символах.
    bool m_nameConfirmed = false;

    sf::Sound m_moveSound;
    sf::Sound m_confirmSound;
    sf::Sound m_winSound;

    void handleNameInput(const sf::Event& event);
    void handleReplayInput(const sf::Event& event);
    void drawNameEntry(sf::RenderWindow& window);
    void drawReplayPrompt(sf::RenderWindow& window);

    void toggleSelection();
    void confirm(Choice choice);

    // Индекс варианта (0 - Да, 1 - Нет) под курсором мыши или -1.
    int itemIndexAt(int mouseY) const;
};
