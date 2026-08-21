#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>

class MenuState : public IState {
public:
    explicit MenuState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;
    bool wantsExit() const override { return m_exitRequested; }

private:
    enum class MenuItem { Continue, StartGame, Difficulty, Highscores, Help, Settings, Exit };
    static const int ITEM_COUNT = 7;

    static bool saveFileExists();

    GameContext& m_context;
    StaleKeyGuard m_staleKeys;

    // Проверяется на диске один раз при входе в меню, а не в каждом draw().
    bool m_hasSave;

    MenuItem m_selected;

    bool m_confirmed = false;
    bool m_exitRequested = false;

    sf::Sound m_moveSound;
    sf::Sound m_confirmSound;

    void moveSelection(int direction);
    static const char* itemLabel(MenuItem item);

    // Активирует то, что сейчас выбрано.
    void activateSelected();

    // Индекс пункта под курсором мыши (по Y) или -1.
    int itemIndexAt(int mouseY) const;
};
