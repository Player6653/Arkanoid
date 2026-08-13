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

MenuState::MenuState(GameContext& context)
    : m_context(context)
{
    m_moveSound.setBuffer(m_context.resources.getRotateBuffer());
    m_confirmSound.setBuffer(m_context.resources.getLineClearBuffer());

    switchMusic(m_context.settings, m_context.resources.getGameplayMusic(), m_context.resources.getMenuMusic());
}

void MenuState::moveSelection(int direction)
{
    int index = static_cast<int>(m_selected);
    index = (index + direction + ITEM_COUNT) % ITEM_COUNT;
    m_selected = static_cast<MenuItem>(index);

    playSfx(m_context.settings, m_moveSound);
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
            if (m_selected == MenuItem::Exit) {
                m_exitRequested = true;
            }
            else {
                playSfx(m_context.settings, m_confirmSound);
                m_confirmed = true;
            }
            break;

        default:
            break;
    }
}

void MenuState::draw(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.resources.getFont();

    drawText(window, font, "ТЕТРИС", 40.f, 40.f, 36);

    float y = 140.f;
    for (int i = 0; i < ITEM_COUNT; ++i) {
        MenuItem item = static_cast<MenuItem>(i);
        bool selected = (item == m_selected);
        std::string text = (selected ? "> " : "  ") + std::string(itemLabel(item));
        drawText(window, font, text, 40.f, y, 24, selected ? sf::Color::Yellow : sf::Color::White);
        y += 40.f;
    }
}

std::unique_ptr<IState> MenuState::nextState()
{
    if (!m_confirmed) {
        return nullptr;
    }

    switch (m_selected) {
        case MenuItem::StartGame:
            return std::make_unique<PlayingState>(m_context, m_context.settings.difficulty);
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
