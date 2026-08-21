#include "GameOverState.h"
#include "MenuState.h"
#include "../Leaderboard.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include "../NameEntry.h"
#include "../SavePaths.h"
#include <string>

GameOverState::GameOverState(GameContext& context, int finalScore)
    : m_context(context)
    , m_finalScore(finalScore)
{
    m_context.getResources().getGameplayMusic().stop();

    m_gameOverSound.setBuffer(m_context.getResources().getGameOverBuffer());
    playSfx(m_context.getSettings(), m_gameOverSound);

    // Забег закончился поражением — старое сохранение продолжать уже нельзя.
    deleteSaveFile();
}

void GameOverState::handleInput(const sf::Event& event)
{
    if (event.type == sf::Event::TextEntered) {
        NameEntry::handleTextEntered(event.text.unicode, m_enteredName, m_enteredNameLength);
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter && !m_enteredName.empty() && !m_confirmed) {
            m_context.getLeaderboard().addEntry(m_enteredName, m_finalScore);
            m_confirmed = true;
        }
    }
}

void GameOverState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

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
