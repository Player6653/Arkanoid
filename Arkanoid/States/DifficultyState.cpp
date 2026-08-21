#include "DifficultyState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Input.h"
#include "../Audio.h"
#include "../Settings.h"
#include <string>

namespace {
    // Совпадают с координатами строк в draw().
    const float ITEM_START_Y = 140.f;
    const float ITEM_SPACING = 40.f;
    const float ITEM_CLICK_HEIGHT = 32.f;
}

DifficultyState::DifficultyState(GameContext& context)
    : m_context(context)
{
    m_changeSound.setBuffer(m_context.getResources().getSwapBuffer());
}

void DifficultyState::moveSelection(int direction)
{
    int index = static_cast<int>(m_selectedItem);
    index = (index + direction + ITEM_COUNT) % ITEM_COUNT;
    m_selectedItem = static_cast<SelectedItem>(index);

    playSfx(m_context.getSettings(), m_changeSound);
}

void DifficultyState::changeValue(int direction)
{
    switch (m_selectedItem) {
        case SelectedItem::Difficulty:
            if (direction < 0) {
                m_context.getSettings().decreaseDifficulty();
            }
            else {
                m_context.getSettings().increaseDifficulty();
            }
            break;

        case SelectedItem::BrickRows:
            if (direction < 0) {
                m_context.getSettings().decreaseBrickRows();
            }
            else {
                m_context.getSettings().increaseBrickRows();
            }
            break;

        case SelectedItem::LevelCount:
            if (direction < 0) {
                m_context.getSettings().decreaseLevelCount();
            }
            else {
                m_context.getSettings().increaseLevelCount();
            }
            break;
    }

    // Значение зациклено и меняется всегда, так что звук играет на каждый клик.
    playSfx(m_context.getSettings(), m_changeSound);
}

int DifficultyState::itemIndexAt(int mouseY) const
{
    for (int i = 0; i < ITEM_COUNT; ++i) {
        float top = ITEM_START_Y + i * ITEM_SPACING;
        if (mouseY >= top && mouseY < top + ITEM_CLICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

void DifficultyState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (event.type == sf::Event::MouseMoved) {
        int index = itemIndexAt(event.mouseMove.y);
        if (index >= 0) {
            m_selectedItem = static_cast<SelectedItem>(index);
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            m_backRequested = true;
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = itemIndexAt(event.mouseButton.y);
            if (index >= 0) {
                m_selectedItem = static_cast<SelectedItem>(index);
                changeValue(1);
            }
        }
        return;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        int index = itemIndexAt(event.mouseWheelScroll.y);
        if (index >= 0) {
            m_selectedItem = static_cast<SelectedItem>(index);
            changeValue(event.mouseWheelScroll.delta > 0.f ? 1 : -1);
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
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            moveSelection(-1);
            break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            moveSelection(1);
            break;

        case sf::Keyboard::Left:
        case sf::Keyboard::A:
            changeValue(-1);
            break;

        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            changeValue(1);
            break;

        default:
            break;
    }
}

void DifficultyState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "СЛОЖНОСТЬ", 40.f, 40.f, 32);

    const char* multiplierLabels[3] = { "x1", "x1.25", "x1.5" };
    int difficultyIndex = m_context.getSettings().getDifficulty() - 1; // 0-2

    bool difficultySelected = (m_selectedItem == SelectedItem::Difficulty);
    std::string difficultyLine = std::string(difficultySelected ? "> " : "  ")
        + "Сложность: " + difficultyName(m_context.getSettings().getDifficulty())
        + " (очки " + multiplierLabels[difficultyIndex] + ")";
    drawText(window, font, difficultyLine, 40.f, ITEM_START_Y, 22,
        difficultySelected ? sf::Color::Yellow : sf::Color::White);

    bool brickRowsSelected = (m_selectedItem == SelectedItem::BrickRows);
    std::string brickRowsLine = std::string(brickRowsSelected ? "> " : "  ")
        + "Рядов блоков: " + std::to_string(m_context.getSettings().getBrickRows()) + " / 10";
    drawText(window, font, brickRowsLine, 40.f, ITEM_START_Y + ITEM_SPACING, 22,
        brickRowsSelected ? sf::Color::Yellow : sf::Color::White);

    bool levelCountSelected = (m_selectedItem == SelectedItem::LevelCount);
    std::string levelCountLine = std::string(levelCountSelected ? "> " : "  ")
        + "Уровней для победы: " + std::to_string(m_context.getSettings().getLevelCount()) + " / 10";
    drawText(window, font, levelCountLine, 40.f, ITEM_START_Y + 2 * ITEM_SPACING, 22,
        levelCountSelected ? sf::Color::Yellow : sf::Color::White);

    drawText(window, font, "Сложность влияет на скорость шарика.", 40.f, 270.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Больше рядов блоков - длиннее первый уровень.", 40.f, 290.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "С каждым уровнем рядов на одно больше (максимум 10).", 40.f, 310.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Вверх/вниз — выбор пункта, влево/вправо — изменить значение", 40.f, 350.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Мышь: клик ЛКМ увеличивает, колесо — оба направления", 40.f, 370.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Esc/Enter/Space/ПКМ — назад", 40.f, 390.f, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> DifficultyState::nextState()
{
    if (m_backRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
