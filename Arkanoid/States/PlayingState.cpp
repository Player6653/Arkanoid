#include "PlayingState.h"
#include "GameOverState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include "../Settings.h"
#include <string>

PlayingState::PlayingState(GameContext& context, int difficulty)
    : m_context(context)
    , m_gameState(difficulty)
{
    m_level = difficulty;

    m_pauseMoveSound.setBuffer(m_context.getResources().getRotateBuffer());
    m_pauseToggleSound.setBuffer(m_context.getResources().getSwapBuffer());

    switchMusic(m_context.getSettings(), m_context.getResources().getMenuMusic(), m_context.getResources().getGameplayMusic());
}

void PlayingState::setPaused(bool paused)
{
    m_paused = paused;

    if (m_paused) {
        m_context.getResources().getGameplayMusic().pause();
    }
    else if (m_context.getSettings().isMusicOn()) {
        playIfNotPlaying(m_context.getResources().getGameplayMusic());
    }
}

void PlayingState::movePauseSelection(int direction)
{
    int index = static_cast<int>(m_pauseSelected);
    index = (index + direction + PAUSE_ITEM_COUNT) % PAUSE_ITEM_COUNT;
    m_pauseSelected = static_cast<PauseItem>(index);

    playSfx(m_context.getSettings(), m_pauseMoveSound);
}

void PlayingState::activatePauseSelection()
{
    switch (m_pauseSelected) {
        case PauseItem::Resume:
            setPaused(false);
            break;

        case PauseItem::ToggleSound:
            m_context.getSettings().toggleSound();
            playSfx(m_context.getSettings(), m_pauseToggleSound);
            break;

        case PauseItem::ToggleMusic:
            m_context.getSettings().toggleMusic();
            if (m_context.getSettings().isMusicOn()) {
                playIfNotPlaying(m_context.getResources().getGameplayMusic());
            }
            else {
                m_context.getResources().getGameplayMusic().pause();
            }
            playSfx(m_context.getSettings(), m_pauseToggleSound);
            break;

        case PauseItem::QuitToMenu:
            m_quitToMenuRequested = true;
            break;
    }
}

std::string PlayingState::pauseItemLabel(PauseItem item) const
{
    switch (item) {
        case PauseItem::Resume:
            return "Продолжить";
        case PauseItem::ToggleSound:
            return std::string("Звук: ") + (m_context.getSettings().isSoundOn() ? "ВКЛ" : "ВЫКЛ");
        case PauseItem::ToggleMusic:
            return std::string("Музыка: ") + (m_context.getSettings().isMusicOn() ? "ВКЛ" : "ВЫКЛ");
        case PauseItem::QuitToMenu:
            return "Выйти в меню";
    }
    return "";
}

void PlayingState::handlePausedInput(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    switch (event.key.code) {
        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            movePauseSelection(-1);
            break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            movePauseSelection(1);
            break;

        case sf::Keyboard::Enter:
            if (!m_pauseEnterKeyHeld) {
                m_pauseEnterKeyHeld = true;
                activatePauseSelection();
            }
            break;

        case sf::Keyboard::Space:
            if (!m_spaceKeyHeld) {
                m_spaceKeyHeld = true;
                activatePauseSelection();
            }
            break;

        default:
            break;
    }
}

void PlayingState::handleInput(const sf::Event& event)
{
    // обрабатывает всегда, независимо от паузы.
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Space) {
            m_spaceKeyHeld = false;
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            m_escapeKeyHeld = false;
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            m_pauseEnterKeyHeld = false;
        }
        return;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (!m_escapeKeyHeld) {
            m_escapeKeyHeld = true;
            setPaused(!m_paused);
        }
        return;
    }

    if (m_paused) {
        handlePausedInput(event);
    }
    // Платформа опрашивает ввод в update(), поэтому во время игры (не на паузе) обрабатывать тут больше нечего.
}

void PlayingState::update(sf::Time dt, const sf::RenderWindow& window)
{
    if (m_paused) {
        return;
    }

    m_gameState.update(dt, window);

    if (m_gameState.isBallLost()) {
        m_gameOver = true;
    }
}

void PlayingState::draw(sf::RenderWindow& window)
{
    m_gameState.draw(window);

    sf::RectangleShape separator(sf::Vector2f(2.f, static_cast<float>(ROWS * TILE_SIZE)));
    separator.setPosition(static_cast<float>(COLS * TILE_SIZE), 0.f);
    separator.setFillColor(sf::Color(90, 90, 90));
    window.draw(separator);

    const sf::Font& font = m_context.getResources().getFont();
    float panelX = COLS * TILE_SIZE + 20.f;

    drawText(window, font, "Счёт", panelX, 40.f, 18, sf::Color(180, 180, 180));
    drawText(window, font, std::to_string(m_score), panelX, 62.f, 28);

    drawText(window, font, "Сложность", panelX, 130.f, 18, sf::Color(180, 180, 180));
    drawText(window, font, difficultyName(m_level), panelX, 152.f, 22);

    if (m_paused) {
        drawPauseOverlay(window);
    }
}

void PlayingState::drawPauseOverlay(sf::RenderWindow& window)
{
    sf::RectangleShape dim(sf::Vector2f(
        static_cast<float>(COLS * TILE_SIZE + UI_PANEL_WIDTH),
        static_cast<float>(ROWS * TILE_SIZE)));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(dim);

    const sf::Font& font = m_context.getResources().getFont();
    float x = 60.f;
    float y = 200.f;

    drawText(window, font, "ПАУЗА", x, y, 36);
    y += 70.f;

    for (int i = 0; i < PAUSE_ITEM_COUNT; ++i) {
        PauseItem item = static_cast<PauseItem>(i);
        bool selected = (item == m_pauseSelected);
        std::string text = (selected ? "> " : "  ") + pauseItemLabel(item);
        drawText(window, font, text, x, y, 24, selected ? sf::Color::Yellow : sf::Color::White);
        y += 40.f;
    }
}

std::unique_ptr<IState> PlayingState::nextState()
{
    if (m_gameOver) {
        return std::make_unique<GameOverState>(m_context, m_score);
    }
    if (m_quitToMenuRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    return nullptr;
}
