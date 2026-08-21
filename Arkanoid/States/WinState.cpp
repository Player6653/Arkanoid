#include "WinState.h"
#include "MenuState.h"
#include "PlayingState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include "../Leaderboard.h"
#include "../NameEntry.h"
#include "../SavePaths.h"
#include <string>

namespace {
    // Совпадают с координатами строк Да/Нет.
    const float ITEM_START_Y = 270.f;
    const float ITEM_SPACING = 40.f;
    const float ITEM_CLICK_HEIGHT = 32.f;
}

WinState::WinState(GameContext& context, int score)
    : m_context(context)
    , m_score(score)
{
    m_moveSound.setBuffer(m_context.getResources().getRotateBuffer());
    m_confirmSound.setBuffer(m_context.getResources().getLineClearBuffer());

    m_context.getResources().getGameplayMusic().stop();

    m_winSound.setBuffer(m_context.getResources().getWinBuffer());
    playSfx(m_context.getSettings(), m_winSound);

    // Забег закончился победой — продолжать уже нечего, старое сохранение удаляем.
    deleteSaveFile();
}

void WinState::toggleSelection()
{
    m_selected = (m_selected == Choice::Yes) ? Choice::No : Choice::Yes;
    playSfx(m_context.getSettings(), m_moveSound);
}

void WinState::confirm(Choice choice)
{
    m_selected = choice;
    m_confirmed = true;
    playSfx(m_context.getSettings(), m_confirmSound);
}

int WinState::itemIndexAt(int mouseY) const
{
    for (int i = 0; i < 2; ++i) {
        float top = ITEM_START_Y + i * ITEM_SPACING;
        if (mouseY >= top && mouseY < top + ITEM_CLICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

void WinState::handleNameInput(const sf::Event& event)
{
    if (event.type == sf::Event::TextEntered) {
        NameEntry::handleTextEntered(event.text.unicode, m_enteredName, m_enteredNameLength);
        return;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && !m_enteredName.empty()) {
        m_context.getLeaderboard().addEntry(m_enteredName, m_score);
        m_nameConfirmed = true;

        // Пересоздаём гвард.
        m_staleKeys = StaleKeyGuard();
    }
}

void WinState::handleReplayInput(const sf::Event& event)
{
    if (!m_staleKeys.accept(event)) {
        return;
    }

    if (event.type == sf::Event::MouseMoved) {
        int index = itemIndexAt(event.mouseMove.y);
        if (index >= 0) {
            m_selected = (index == 0) ? Choice::Yes : Choice::No;
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            confirm(Choice::No);
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = itemIndexAt(event.mouseButton.y);
            if (index >= 0) {
                confirm(index == 0 ? Choice::Yes : Choice::No);
            }
        }
        return;
    }

    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    switch (event.key.code) {
        case sf::Keyboard::Left:
        case sf::Keyboard::Right:
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
        case sf::Keyboard::Tab:
            toggleSelection();
            break;

        case sf::Keyboard::Y:
            confirm(Choice::Yes);
            break;

        case sf::Keyboard::N:
            confirm(Choice::No);
            break;

        case sf::Keyboard::Enter:
        case sf::Keyboard::Space:
            confirm(m_selected);
            break;

        default:
            break;
    }
}

void WinState::handleInput(const sf::Event& event)
{
    if (!m_nameConfirmed) {
        handleNameInput(event);
    }
    else {
        handleReplayInput(event);
    }
}

void WinState::drawNameEntry(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "ПОБЕДА!", 40.f, 40.f, 36, sf::Color::Yellow);
    drawText(window, font, "Вы разбили все блоки на всех уровнях!", 40.f, 100.f, 22);
    drawText(window, font, "Счёт: " + std::to_string(m_score), 40.f, 136.f, 22);

    drawText(window, font, "Введите имя для таблицы рекордов:", 40.f, 190.f, 20);
    drawText(window, font, m_enteredName + "_", 40.f, 220.f, 24, sf::Color::Yellow);
    drawText(window, font, "Enter — подтвердить", 40.f, 300.f, 16, sf::Color(180, 180, 180));
}

void WinState::drawReplayPrompt(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();

    drawText(window, font, "ПОБЕДА!", 40.f, 40.f, 36, sf::Color::Yellow);
    drawText(window, font, "Счёт: " + std::to_string(m_score), 40.f, 100.f, 22);

    drawText(window, font, "Сыграть ещё раз?", 40.f, 220.f, 24);

    std::string yesLabel = (m_selected == Choice::Yes ? "> " : "  ") + std::string("Да (Y)");
    std::string noLabel = (m_selected == Choice::No ? "> " : "  ") + std::string("Нет (N)");
    drawText(window, font, yesLabel, 40.f, ITEM_START_Y, 22, m_selected == Choice::Yes ? sf::Color::Yellow : sf::Color::White);
    drawText(window, font, noLabel, 40.f, ITEM_START_Y + ITEM_SPACING, 22, m_selected == Choice::No ? sf::Color::Yellow : sf::Color::White);

    drawText(window, font, "Стрелки/Tab/клик — выбор, Enter/Space — подтвердить, ПКМ — Нет", 40.f, 560.f, 16, sf::Color(180, 180, 180));
}

void WinState::draw(sf::RenderWindow& window)
{
    if (!m_nameConfirmed) {
        drawNameEntry(window);
    }
    else {
        drawReplayPrompt(window);
    }
}

std::unique_ptr<IState> WinState::nextState()
{
    if (!m_confirmed) {
        return nullptr;
    }

    if (m_selected == Choice::Yes) {
        return std::make_unique<PlayingState>(m_context, m_context.getSettings().getDifficulty());
    }
    return std::make_unique<MenuState>(m_context);
}
