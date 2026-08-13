#include "HighscoresState.h"
#include "MenuState.h"
#include "../Leaderboard.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include <string>

HighscoresState::HighscoresState(GameContext& context)
    : m_context(context)
{
}

void HighscoresState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (isBackKey(event)) {
        m_backRequested = true;
    }
}

void HighscoresState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();
    const std::vector<HighscoreEntry>& entries = m_context.leaderboard.getEntries();

    drawText(window, font, "ТАБЛИЦА РЕКОРДОВ", 40.f, 40.f, 32);

    if (entries.empty()) {
        drawText(window, font, "(пусто)", 40.f, 140.f, 20);
    }
    else {
        float y = 140.f;
        int shown = 0;
        for (const HighscoreEntry& entry : entries) {
            if (shown >= 8) {
                break;
            }
            std::string line = std::to_string(shown + 1) + ". " + entry.name + " - " + std::to_string(entry.score);
            drawText(window, font, line, 40.f, y, 20);
            y += 30.f;
            ++shown;
        }
    }

    drawText(window, font, "Esc/Enter/Space — назад", 40.f, 550.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> HighscoresState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
