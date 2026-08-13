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
    m_changeSound.setBuffer(m_context.resources.getSwapBuffer());
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
            if (m_context.settings.difficulty > 1) {
                m_context.settings.difficulty--;
                playSfx(m_context.settings, m_changeSound);
            }
            break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            if (m_context.settings.difficulty < 3) {
                m_context.settings.difficulty++;
                playSfx(m_context.settings, m_changeSound);
            }
            break;

        default:
            break;
    }
}

void DifficultyState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();
    const char* labels[3] = {
        "1) Лёгкий  - медленное падение, x1 к очкам",
        "2) Средний - среднее падение, x2 к очкам",
        "3) Сложный - быстрое падение, x3 к очкам"
    };

    drawText(window, font, "СЛОЖНОСТЬ", 40.f, 40.f, 32);

    for (int i = 0; i < 3; ++i) {
        bool selected = (m_context.settings.difficulty == i + 1);
        drawText(window, font, labels[i], 40.f, 140.f + i * 40.f, 18,
            selected ? sf::Color::Yellow : sf::Color::White);
    }

    drawText(window, font, "Сложность влияет на скорость падения фигур", 40.f, 290.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "и на множитель начисляемых очков.", 40.f, 310.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Вверх/вниз - менять, Esc/Enter/Space - назад", 40.f, 350.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> DifficultyState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
