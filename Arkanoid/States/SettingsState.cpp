#include "SettingsState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include "../Audio.h"
#include <string>

namespace {
    // Совпадают с координатами строк в draw().
    const float ITEM_START_Y = 140.f;
    const float ITEM_SPACING = 40.f;
    const float ITEM_CLICK_HEIGHT = 32.f;
}

SettingsState::SettingsState(GameContext& context, sf::Music& activeMusic,
    std::function<std::unique_ptr<IState>()> onBack)
    : m_context(context)
    , m_activeMusic(activeMusic)
    , m_onBack(std::move(onBack))
{
    m_toggleSound.setBuffer(m_context.getResources().getSwapBuffer());
}

void SettingsState::toggleSoundSetting()
{
    m_selectedItem = SelectedItem::Sound;
    m_context.getSettings().toggleSound();
    playSfx(m_context.getSettings(), m_toggleSound);
}

void SettingsState::toggleMusicSetting()
{
    m_selectedItem = SelectedItem::Music;
    m_context.getSettings().toggleMusic();
    if (m_context.getSettings().isMusicOn()) {
        playIfNotPlaying(m_activeMusic);
    }
    else {
        m_activeMusic.pause();
    }
    playSfx(m_context.getSettings(), m_toggleSound);
}

void SettingsState::moveSelection(int direction)
{
    int index = static_cast<int>(m_selectedItem);
    index = (index + direction + ITEM_COUNT) % ITEM_COUNT;
    m_selectedItem = static_cast<SelectedItem>(index);
}

void SettingsState::changeValue(int direction)
{
    switch (m_selectedItem) {
        case SelectedItem::Sound:
            toggleSoundSetting();
            break;

        case SelectedItem::Music:
            toggleMusicSetting();
            break;

        case SelectedItem::FpsLimit:
            if (direction < 0) {
                m_context.getSettings().decreaseFpsLimit();
            }
            else {
                m_context.getSettings().increaseFpsLimit();
            }
            playSfx(m_context.getSettings(), m_toggleSound);
            break;
    }
}

int SettingsState::itemIndexAt(int mouseY) const
{
    for (int i = 0; i < ITEM_COUNT; ++i) {
        float top = ITEM_START_Y + i * ITEM_SPACING;
        if (mouseY >= top && mouseY < top + ITEM_CLICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

void SettingsState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (event.type == sf::Event::MouseMoved) {
        int index = itemIndexAt(event.mouseMove.y);
        if (index >= 0) {
            m_selectedItem = static_cast<SelectedItem>(index);
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        // ПКМ — везде назад.
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_backRequested = true;
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = itemIndexAt(event.mouseButton.y);
            if (index >= 0) {
                m_selectedItem = static_cast<SelectedItem>(index);
                changeValue(1);
            }
        }
        return;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        int index = itemIndexAt(event.mouseWheelScroll.y);
        if (index >= 0) {
            m_selectedItem = static_cast<SelectedItem>(index);
            changeValue(event.mouseWheelScroll.delta > 0.f ? 1 : -1);
        }
        return;
    }

    if (isBackKey(event)) {
        m_backRequested = true;
        return;
    }

    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    switch (event.key.code) {
        case sf::Keyboard::Up:
            moveSelection(-1);
            break;

        case sf::Keyboard::Down:
            moveSelection(1);
            break;

        case sf::Keyboard::Left:
            changeValue(-1);
            break;

        case sf::Keyboard::Right:
            changeValue(1);
            break;

        case sf::Keyboard::S:
            toggleSoundSetting();
            break;

        case sf::Keyboard::M:
            toggleMusicSetting();
            break;

        case sf::Keyboard::F:
            m_selectedItem = SelectedItem::FpsLimit;
            changeValue(1);
            break;

        default:
            break;
    }
}

void SettingsState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "НАСТРОЙКИ", 40.f, 40.f, 32);

    bool soundSelected = (m_selectedItem == SelectedItem::Sound);
    bool musicSelected = (m_selectedItem == SelectedItem::Music);
    bool fpsSelected = (m_selectedItem == SelectedItem::FpsLimit);

    std::string sound = std::string(soundSelected ? "> " : "  ")
        + "Звук: " + (m_context.getSettings().isSoundOn() ? "ВКЛ" : "ВЫКЛ") + "  (S / клик)";
    std::string music = std::string(musicSelected ? "> " : "  ")
        + "Музыка: " + (m_context.getSettings().isMusicOn() ? "ВКЛ" : "ВЫКЛ") + "  (M / клик)";

    int fpsLimit = m_context.getSettings().getFpsLimit();
    std::string fpsValue = (fpsLimit == 0) ? "без ограничения" : (std::to_string(fpsLimit) + " FPS");
    std::string fps = std::string(fpsSelected ? "> " : "  ")
        + "Лимит FPS: " + fpsValue + "  (F / клик / влево-вправо)";

    drawText(window, font, sound, 40.f, ITEM_START_Y, 24, soundSelected ? sf::Color::Yellow : sf::Color::White);
    drawText(window, font, music, 40.f, ITEM_START_Y + ITEM_SPACING, 24, musicSelected ? sf::Color::Yellow : sf::Color::White);
    drawText(window, font, fps, 40.f, ITEM_START_Y + 2 * ITEM_SPACING, 24, fpsSelected ? sf::Color::Yellow : sf::Color::White);

    drawText(window, font, "Вверх/вниз — выбор пункта, влево/вправо/колесо — изменить значение", 40.f, ITEM_START_Y + 3 * ITEM_SPACING + 20.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Esc/Enter/Space/ПКМ — назад", 40.f, ITEM_START_Y + 3 * ITEM_SPACING + 40.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> SettingsState::nextState()
{
    if (m_backRequested) {
        return m_onBack ? m_onBack() : std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
