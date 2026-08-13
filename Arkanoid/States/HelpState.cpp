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

    if (isBackKey(event)) {
        m_backRequested = true;
    }
}

void HelpState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();

    drawText(window, font, "ПОМОЩЬ", 40.f, 40.f, 32);

    float y = 100.f;
    drawText(window, font, "Управление:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 34.f;
    drawText(window, font, "Влево / A - сдвиг влево", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Вправо / D - сдвиг вправо", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Вниз / S - ускоренное падение", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Вверх / W - поворот фигуры", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Space - мгновенный сброс (Hard Drop)", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "C - карман (Hold)", 40.f, y, 18);
    y += 26.f;
    drawText(window, font, "Esc - пауза", 40.f, y, 18);
    y += 44.f;

    drawText(window, font, "Подсказки:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 34.f;
    drawText(window, font, "Тень внизу стакана (Ghost Piece) показывает,", 40.f, y, 18);
    y += 24.f;
    drawText(window, font, "куда упадёт текущая фигура.", 40.f, y, 18);
    y += 34.f;
    drawText(window, font, "Карман (Hold) откладывает фигуру про запас -", 40.f, y, 18);
    y += 24.f;
    drawText(window, font, "использовать снова можно только после", 40.f, y, 18);
    y += 24.f;
    drawText(window, font, "фиксации текущей фигуры.", 40.f, y, 18);

    drawText(window, font, "Esc/Enter/Space - назад", 40.f, 560.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> HelpState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
