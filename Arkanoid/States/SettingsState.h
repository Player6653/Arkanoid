// States/SettingsState.h — экран настроек.
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Input.h"
#include <SFML/Audio.hpp>
#include <functional>
#include <memory>

// Экран настроек. Используется и из главного меню, и из паузы во время игры.
class SettingsState : public IState {
public:
    explicit SettingsState(GameContext& context, sf::Music& activeMusic,
        std::function<std::unique_ptr<IState>()> onBack = nullptr);

    void handleInput(const sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    enum class SelectedItem { Sound, Music, FpsLimit };
    static const int ITEM_COUNT = 3;

    GameContext& m_context;
    sf::Music& m_activeMusic;
    std::function<std::unique_ptr<IState>()> m_onBack;
    StaleKeyGuard m_staleKeys;
    bool m_backRequested = false;
    sf::Sound m_toggleSound;
    SelectedItem m_selectedItem = SelectedItem::Sound;

    void toggleSoundSetting();
    void toggleMusicSetting();

    // Переключает, какой из пунктов сейчас выбран.
    void moveSelection(int direction);

    // Меняет значение текущего выбранного пункта.
    void changeValue(int direction);

    // Индекс пункта (0 - звук, 1 - музыка, 2 - лимит FPS) под курсором мыши или -1.
    int itemIndexAt(int mouseY) const;
};
