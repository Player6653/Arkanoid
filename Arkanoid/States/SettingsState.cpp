#include "SettingsState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include "../Audio.h"
#include <string>

namespace {
    // Совпадают с координатами строк в draw()ю
    const float ITEM_START_Y = 140.f;
    const float ITEM_SPACING = 40.f;
    const float ITEM_CLICK_HEIGHT = 32.f;
}

SettingsState::SettingsState(GameContext& context)
    : m_context(context)
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
        playIfNotPlaying(m_context.getResources().getMenuMusic());
    }
    else {
        m_context.getResources().getMenuMusic().pause();
    }
    playSfx(m_context.getSettings(), m_toggleSound);
}

int SettingsState::itemIndexAt(int mouseY) const
{
    for (int i = 0; i < 2; ++i) {
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
        if (index == 0) {
            m_selectedItem = SelectedItem::Sound;
        }
        else if (index == 1) {
            m_selectedItem = SelectedItem::Music;
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        // ПКМ — везде единый назад.
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_backRequested = true;
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = itemIndexAt(event.mouseButton.y);
            if (index == 0) {
                toggleSoundSetting();
            }
            else if (index == 1) {
                toggleMusicSetting();
            }
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
        case sf::Keyboard::S:
            toggleSoundSetting();
            break;

        case sf::Keyboard::M:
            toggleMusicSetting();
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

    std::string sound = std::string(soundSelected ? "> " : "  ")
        + "Звук: " + (m_context.getSettings().isSoundOn() ? "ВКЛ" : "ВЫКЛ") + "  (S / клик)";
    std::string music = std::string(musicSelected ? "> " : "  ")
        + "Музыка: " + (m_context.getSettings().isMusicOn() ? "ВКЛ" : "ВЫКЛ") + "  (M / клик)";
    drawText(window, font, sound, 40.f, ITEM_START_Y, 24, soundSelected ? sf::Color::Yellow : sf::Color::White);
    drawText(window, font, music, 40.f, ITEM_START_Y + ITEM_SPACING, 24, musicSelected ? sf::Color::Yellow : sf::Color::White);

    drawText(window, font, "Esc/Enter/Space/ПКМ — назад", 40.f, 300.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> SettingsState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
