// States/PlayingState.h — экран игры.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../GameState.h"
#include <SFML/Audio.hpp>
#include <string>

// экран игры.
class PlayingState : public IState {
public:
    PlayingState(GameContext& context, int difficulty);

    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt, const sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    GameState m_gameState;

    int m_score = 0;
    int m_level = 1;

    bool m_gameOver = false;
    bool m_won = false;

    static const int SCORE_PER_BRICK = 10;

    bool m_spaceKeyHeld = false;
    bool m_escapeKeyHeld = false;
    bool m_pauseEnterKeyHeld = false;

    enum class PauseItem { Resume, ToggleSound, ToggleMusic, QuitToMenu };
    static const int PAUSE_ITEM_COUNT = 4;

    bool m_paused = false;
    PauseItem m_pauseSelected = PauseItem::Resume;
    bool m_quitToMenuRequested = false;

    sf::Sound m_pauseMoveSound;
    sf::Sound m_pauseToggleSound;

    void handlePausedInput(const sf::Event& event);

    void setPaused(bool paused);

    void movePauseSelection(int direction);
    void activatePauseSelection();
    std::string pauseItemLabel(PauseItem item) const;
    void drawPauseOverlay(sf::RenderWindow& window);

    // какой цвет что значит.
    void drawBrickLegend(sf::RenderWindow& window);

    // Индекс пункта меню паузы под курсором мыши (по Y) или -1.
    int pauseItemIndexAt(int mouseY) const;
};
