#include "GameOverState.h"
#include "MenuState.h"
#include "../Leaderboard.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include <SFML/System/Utf.hpp>
#include <string>
#include <iterator>

namespace {
    const sf::Uint32 BACKSPACE = 8;
    const int MAX_NAME_LENGTH = 12; // в символах

    // Можно по русски писать.
    bool isAllowedNameChar(sf::Uint32 unicode)
    {
        bool isAsciiPrintable = unicode > 32 && unicode < 127;
        bool isCyrillic = unicode >= 0x0400 && unicode <= 0x04FF;
        return isAsciiPrintable || isCyrillic;
    }

    void popLastUtf8Char(std::string& s)
    {
        if (s.empty()) {
            return;
        }
        std::size_t i = s.size() - 1;
        while (i > 0 && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) {
            --i;
        }
        s.erase(i);
    }
}

GameOverState::GameOverState(GameContext& context, int finalScore)
    : m_context(context)
    , m_finalScore(finalScore)
{
    m_context.resources.getGameplayMusic().stop();

    m_gameOverSound.setBuffer(m_context.resources.getGameOverBuffer());
    playSfx(m_context.settings, m_gameOverSound);
}

void GameOverState::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == BACKSPACE) {
            if (!m_enteredName.empty()) {
                popLastUtf8Char(m_enteredName);
                --m_enteredNameLength;
            }
        }
        else if (isAllowedNameChar(event.text.unicode)) {
            if (m_enteredNameLength < MAX_NAME_LENGTH) {
                sf::Utf8::encode(event.text.unicode, std::back_inserter(m_enteredName));
                ++m_enteredNameLength;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter && !m_enteredName.empty() && !m_confirmed) {
            m_context.leaderboard.addEntry(m_enteredName, m_finalScore);
            m_confirmed = true;
        }
    }
}

void GameOverState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();

    drawText(window, font, "ИГРА ОКОНЧЕНА", 40.f, 40.f, 36, sf::Color::Red);
    drawText(window, font, "Счёт: " + std::to_string(m_finalScore), 40.f, 100.f, 24);
    drawText(window, font, "Введите имя:", 40.f, 160.f, 20);
    drawText(window, font, m_enteredName + "_", 40.f, 190.f, 24, sf::Color::Yellow);
    drawText(window, font, "Enter — подтвердить", 40.f, 300.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> GameOverState::nextState()
{
    if (m_confirmed) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
