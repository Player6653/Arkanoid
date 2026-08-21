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
    y += 30.f;
    drawText(window, font, "Стрелки влево/вправо или мышь - движение платформы", 40.f, y, 17);
    y += 24.f;
    drawText(window, font, "Esc/ПКМ - пауза (там же можно сохранить игру)", 40.f, y, 17);
    y += 38.f;

    drawText(window, font, "Блоки:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 30.f;
    drawText(window, font, "Обычный - разбивается с одного удара", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Крепкий - выдерживает несколько ударов, темнеет с каждым", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Стекло - мяч пролетает сквозь него, не отражаясь", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Неразрушимый - не разбивается никогда, для победы не нужен", 40.f, y, 17);
    y += 38.f;

    drawText(window, font, "Бонусы (падают с разбитого блока с шансом 10%):", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 30.f;
    drawText(window, font, "Огненный мяч (7 сек) - разгоняется, пробивает блоки, мяч краснеет", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Хрупкие блоки (6 сек) - любой блок бьётся с одного удара, мяч синеет", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Широкая платформа (8 сек) - платформа шире, но чуть медленнее", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Динамит (мгновенно) - убирает случайный неразрушимый блок", 40.f, y, 17);
    y += 38.f;

    drawText(window, font, "Как победить:", 40.f, y, 20, sf::Color(200, 200, 200));
    y += 30.f;
    drawText(window, font, "Разбейте все блоки на уровне, чтобы перейти на следующий", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Число уровней для победы задаётся в Уровне сложности", 40.f, y, 17);
    y += 22.f;
    drawText(window, font, "Не дайте шарику улететь ниже платформы - это проигрыш", 40.f, y, 17);

    drawText(window, font, "Esc/Enter/Space/ПКМ - назад", 40.f, 560.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> HelpState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
