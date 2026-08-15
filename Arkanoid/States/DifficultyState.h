// сложность.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>

// Выбор уровня сложности и количества рядов блоков.
class DifficultyState : public IState {
public:
    explicit DifficultyState(GameContext& context);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    enum class SelectedItem { Difficulty, BrickRows };
    static const int ITEM_COUNT = 2;

    GameContext& m_context;
    StaleKeyGuard m_staleKeys;
    SelectedItem m_selectedItem = SelectedItem::Difficulty;
    bool m_backRequested = false;
    sf::Sound m_changeSound;

    // Переключает, какой из двух пунктов сейчас редактируется.
    void moveSelection(int direction);

    // Меняет значение текущего выбранного пункта.
    void changeValue(int direction);

    // Индекс пункта под курсором мыши (по Y) или -1.
    int itemIndexAt(int mouseY) const;
};
