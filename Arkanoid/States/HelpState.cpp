#include "HelpState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"

HelpState::HelpState(GameContext& context)
    : m_context(context)
{
}

void HelpState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        m_backRequested = true;
        return;
    }

    if (isBackKey(event)) {
        m_backRequested = true;
    }
}

void HelpState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "ПОМОЩЬ", 40.f, 40.f, 32);

    float y = 100.f;
    drawText(window, font, "Управление:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 34.f;
    drawText(window, font, "Стрелки влево/вправо - движение платформы", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Мышь - платформа следует за курсором", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Esc - пауза", 40.f, y, 18);
    y += 44.f;

    drawText(window, font, "Подсказки:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 34.f;
    drawText(window, font, "Не давайте шарику улететь ниже платформы -", 40.f, y, 18);
    y += 24.f;
    drawText(window, font, "иначе игра закончится.", 40.f, y, 18);

    drawText(window, font, "Esc/Enter/Space/ПКМ - назад", 40.f, 560.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> HelpState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
