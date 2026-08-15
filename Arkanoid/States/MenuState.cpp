#include "MenuState.h"
#include "PlayingState.h"
#include "DifficultyState.h"
#include "HighscoresState.h"
#include "HelpState.h"
#include "SettingsState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include <string>

namespace {
    // Эти же значения использует draw() для расположения пунктов.
    const float ITEM_START_Y = 140.f;
    const float ITEM_SPACING = 40.f;
    const float ITEM_CLICK_HEIGHT = 32.f;
}

MenuState::MenuState(GameContext& context)
    : m_context(context)
{
    m_moveSound.setBuffer(m_context.getResources().getRotateBuffer());
    m_confirmSound.setBuffer(m_context.getResources().getLineClearBuffer());

    switchMusic(m_context.getSettings(), m_context.getResources().getGameplayMusic(), m_context.getResources().getMenuMusic());
}

void MenuState::moveSelection(int direction)
{
    int index = static_cast<int>(m_selected);
    index = (index + direction + ITEM_COUNT) % ITEM_COUNT;
    m_selected = static_cast<MenuItem>(index);

    playSfx(m_context.getSettings(), m_moveSound);
}

void MenuState::activateSelected()
{
    if (m_selected == MenuItem::Exit) {
        m_exitRequested = true;
    }
    else {
        playSfx(m_context.getSettings(), m_confirmSound);
        m_confirmed = true;
    }
}

int MenuState::itemIndexAt(int mouseY) const
{
    for (int i = 0; i < ITEM_COUNT; ++i) {
        float top = ITEM_START_Y + i * ITEM_SPACING;
        if (mouseY >= top && mouseY < top + ITEM_CLICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

const char* MenuState::itemLabel(MenuItem item)
{
    switch (item) {
        case MenuItem::StartGame:  return "Начать игру";
        case MenuItem::Difficulty: return "Уровень сложности";
        case MenuItem::Highscores: return "Таблица рекордов";
        case MenuItem::Help:       return "Помощь";
        case MenuItem::Settings:   return "Настройки";
        case MenuItem::Exit:       return "Выход";
    }
    return "";
}

void MenuState::handleInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (event.type == sf::Event::MouseMoved) {
        int index = itemIndexAt(event.mouseMove.y);
        if (index >= 0) {
            m_selected = static_cast<MenuItem>(index);
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = itemIndexAt(event.mouseButton.y);
            if (index >= 0) {
                m_selected = static_cast<MenuItem>(index);
                activateSelected();
            }
        }
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

        case sf::Keyboard::Enter:
        case sf::Keyboard::Space:
            activateSelected();
            break;

        default:
            break;
    }
}

void MenuState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "АРКАНОИД", 40.f, 40.f, 36);

    float y = ITEM_START_Y;
    for (int i = 0; i < ITEM_COUNT; ++i) {
        MenuItem item = static_cast<MenuItem>(i);
        bool selected = (item == m_selected);
        std::string text = (selected ? "> " : "  ") + std::string(itemLabel(item));
        drawText(window, font, text, 40.f, y, 24, selected ? sf::Color::Yellow : sf::Color::White);
        y += ITEM_SPACING;
    }
}

std::unique_ptr<IState> MenuState::nextState()
{
    if (!m_confirmed) {
        return nullptr;
    }

    switch (m_selected) {
        case MenuItem::StartGame:
            return std::make_unique<PlayingState>(m_context, m_context.getSettings().getDifficulty());
        case MenuItem::Difficulty:
            return std::make_unique<DifficultyState>(m_context);
        case MenuItem::Highscores:
            return std::make_unique<HighscoresState>(m_context);
        case MenuItem::Help:
            return std::make_unique<HelpState>(m_context);
        case MenuItem::Settings:
            return std::make_unique<SettingsState>(m_context);
        default:
            return nullptr;
    }
}
