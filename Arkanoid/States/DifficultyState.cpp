#include "DifficultyState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include "../Audio.h"
#include <string>

DifficultyState::DifficultyState(GameContext& context)
    : m_context(context)
{
    m_changeSound.setBuffer(m_context.getResources().getSwapBuffer());
}

void DifficultyState::handleInput(const sf::Event& event)
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
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            if (m_context.getSettings().decreaseDifficulty()) {
                playSfx(m_context.getSettings(), m_changeSound);
            }
            break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            if (m_context.getSettings().increaseDifficulty()) {
                playSfx(m_context.getSettings(), m_changeSound);
            }
            break;

        default:
            break;
    }
}

void DifficultyState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();
    const char* labels[3] = {
        "1) Лёгкий  - медленный шарик",
        "2) Средний - средняя скорость шарика",
        "3) Сложный - быстрый шарик"
    };

    drawText(window, font, "СЛОЖНОСТЬ", 40.f, 40.f, 32);

    for (int i = 0; i < 3; ++i) {
        bool selected = (m_context.getSettings().getDifficulty() == i + 1);
        drawText(window, font, labels[i], 40.f, 140.f + i * 40.f, 18,
            selected ? sf::Color::Yellow : sf::Color::White);
    }

    drawText(window, font, "Сложность влияет на скорость шарика.", 40.f, 290.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Вверх/вниз - менять, Esc/Enter/Space - назад", 40.f, 350.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> DifficultyState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
