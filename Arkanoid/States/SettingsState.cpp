#include "SettingsState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include "../Audio.h"
#include <string>

SettingsState::SettingsState(GameContext& context)
    : m_context(context)
{
    m_toggleSound.setBuffer(m_context.resources.getSwapBuffer());
}

void SettingsState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
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
            m_context.settings.soundOn = !m_context.settings.soundOn;
            playSfx(m_context.settings, m_toggleSound); // сыграет, только если звук теперь вкл.
            break;

        case sf::Keyboard::M:
            m_context.settings.musicOn = !m_context.settings.musicOn;
            if (m_context.settings.musicOn) {
                playIfNotPlaying(m_context.resources.getMenuMusic());
            }
            else {
                m_context.resources.getMenuMusic().pause();
            }
            playSfx(m_context.settings, m_toggleSound);
            break;

        default:
            break;
    }
}

void SettingsState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();

    drawText(window, font, "НАСТРОЙКИ", 40.f, 40.f, 32);

    std::string sound = "Звук: " + std::string(m_context.settings.soundOn ? "ВКЛ" : "ВЫКЛ") + "  (S)";
    std::string music = "Музыка: " + std::string(m_context.settings.musicOn ? "ВКЛ" : "ВЫКЛ") + "  (M)";
    drawText(window, font, sound, 40.f, 140.f, 24);
    drawText(window, font, music, 40.f, 180.f, 24);

    drawText(window, font, "Esc/Enter/Space — назад", 40.f, 300.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> SettingsState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
