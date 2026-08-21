// States/PlayingState.h — экран игры.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../GameFacade.h"
#include "../GameMemento.h"
#include "../BonusManager.h"
#include <SFML/Audio.hpp>
#include <string>

// экран игры.
class PlayingState : public IState {
public:
    PlayingState(GameContext& context, int difficulty, int level = 1, int carriedScore = 0);

    // Продолжает игру из файла сохранения (Хранитель).
    static std::unique_ptr<PlayingState> continueFromSave(GameContext& context, const std::string& savePath);

    // Восстанавливает игру из снимка в памяти (Хранитель).
    static std::unique_ptr<PlayingState> fromMemento(GameContext& context, const GameMemento& memento, bool paused,
        const std::vector<BonusManager::ActiveBonusSnapshot>& activeBonuses = {});

    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt, const sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    GameFacade m_facade;

    bool m_gameOver = false;
    bool m_levelComplete = false;

    bool m_spaceKeyHeld = false;
    bool m_escapeKeyHeld = false;
    bool m_pauseEnterKeyHeld = false;

    enum class PauseItem { Resume, SaveGame, OpenSettings, QuitToMenu };
    static const int PAUSE_ITEM_COUNT = 4;

    bool m_paused = false;
    PauseItem m_pauseSelected = PauseItem::Resume;
    bool m_quitToMenuRequested = false;
    bool m_openSettingsRequested = false;
    std::string m_pauseMessage;

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
    void drawBonusLegend(sf::RenderWindow& window);

    // Индекс пункта меню паузы под курсором мыши (по Y) или -1.
    int pauseItemIndexAt(int mouseY) const;

    static int brickRowsForLevel(int baseRows, int level);
};
