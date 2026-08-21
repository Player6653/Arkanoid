#include "PlayingState.h"
#include "GameOverState.h"
#include "WinState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include "../Settings.h"
#include "../BrickColors.h"
#include "../PowerUpColors.h"
#include "../SavePaths.h"
#include <string>

namespace {
    // Совпадают с координатами пунктов меню паузы в drawPauseOverlay().
    const float PAUSE_ITEM_START_Y = 270.f;
    const float PAUSE_ITEM_SPACING = 40.f;
    const float PAUSE_ITEM_CLICK_HEIGHT = 32.f;

    // Общий вид для легенды блоков и легенды бонусов.
    struct LegendEntry { sf::Color color; const char* label; };

    void drawLegend(sf::RenderWindow& window, const sf::Font& font, float panelX, const char* title,
        float startY, const LegendEntry* entries, std::size_t count)
    {
        float y = startY;
        drawText(window, font, title, panelX, y, 16, sf::Color(180, 180, 180));
        y += 22.f;

        for (std::size_t i = 0; i < count; ++i) {
            sf::RectangleShape swatch(sf::Vector2f(14.f, 14.f));
            swatch.setPosition(panelX, y);
            swatch.setFillColor(entries[i].color);
            window.draw(swatch);

            drawText(window, font, entries[i].label, panelX + 22.f, y - 3.f, 13);
            y += 22.f;
        }
    }
}

PlayingState::PlayingState(GameContext& context, int difficulty, int level, int carriedScore)
    : m_context(context)
    , m_facade(difficulty, level, brickRowsForLevel(context.getSettings().getBrickRows(), level),
        context.getSettings().getLevelCount(), carriedScore)
{
    m_pauseMoveSound.setBuffer(m_context.getResources().getRotateBuffer());
    m_pauseToggleSound.setBuffer(m_context.getResources().getSwapBuffer());

    switchMusic(m_context.getSettings(), m_context.getResources().getMenuMusic(), m_context.getResources().getGameplayMusic());
}

std::unique_ptr<PlayingState> PlayingState::continueFromSave(GameContext& context, const std::string& savePath)
{
    auto state = std::make_unique<PlayingState>(context, context.getSettings().getDifficulty());
    // Если файла нет/он повреждён — просто остаётся свежая игра с 1 уровня.
    state->m_facade.loadFromFile(savePath);
    return state;
}

std::unique_ptr<PlayingState> PlayingState::fromMemento(GameContext& context, const GameMemento& memento, bool paused,
    const std::vector<BonusManager::ActiveBonusSnapshot>& activeBonuses)
{
    auto state = std::make_unique<PlayingState>(context, memento.getDifficulty());
    state->m_facade.restore(memento);
    state->m_facade.restoreActiveBonuses(activeBonuses);
    if (paused) {
        state->setPaused(true);
    }
    return state;
}

int PlayingState::brickRowsForLevel(int baseRows, int level)
{
    int rows = baseRows + (level - 1);
    return (rows > 10) ? 10 : rows;
}

void PlayingState::setPaused(bool paused)
{
    m_paused = paused;

    if (m_paused) {
        m_context.getResources().getGameplayMusic().pause();
    }
    else {
        // Пока была пауза, Paddle::update() не вызывался и не видел, как двигалась мышька.
        m_facade.requestPaddleMouseResync();
        m_pauseMessage.clear();

        if (m_context.getSettings().isMusicOn()) {
            playIfNotPlaying(m_context.getResources().getGameplayMusic());
        }
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

        case PauseItem::SaveGame:
            m_pauseMessage = m_facade.saveToFile(savePath()) ? "Игра сохранена" : "Не удалось сохранить";
            playSfx(m_context.getSettings(), m_pauseToggleSound);
            break;

        case PauseItem::OpenSettings:
            m_openSettingsRequested = true;
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
        case PauseItem::SaveGame:
            return "Сохранить игру";
        case PauseItem::OpenSettings:
            return "Настройки";
        case PauseItem::QuitToMenu:
            return "Выйти в меню";
    }
    return "";
}

int PlayingState::pauseItemIndexAt(int mouseY) const
{
    for (int i = 0; i < PAUSE_ITEM_COUNT; ++i) {
        float top = PAUSE_ITEM_START_Y + i * PAUSE_ITEM_SPACING;
        if (mouseY >= top && mouseY < top + PAUSE_ITEM_CLICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

void PlayingState::handlePausedInput(const sf::Event& event)
{
    if (event.type == sf::Event::MouseMoved) {
        int index = pauseItemIndexAt(event.mouseMove.y);
        if (index >= 0) {
            m_pauseSelected = static_cast<PauseItem>(index);
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            setPaused(false);
            return;
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
            int index = pauseItemIndexAt(event.mouseButton.y);
            if (index >= 0) {
                m_pauseSelected = static_cast<PauseItem>(index);
                activatePauseSelection();
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

    if (!m_paused && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        setPaused(true);
        return;
    }

    if (m_paused) {
        handlePausedInput(event);
    }
    // Платформа опрашивает ввод в update().
}

void PlayingState::update(sf::Time dt, const sf::RenderWindow& window)
{
    if (m_paused) {
        return;
    }

    m_facade.update(dt, window);

    if (m_facade.isBallLost()) {
        m_gameOver = true;
    }
    else if (m_facade.isLevelComplete()) {
        m_levelComplete = true;
    }
}

void PlayingState::draw(sf::RenderWindow& window)
{
    m_facade.draw(window);

    sf::RectangleShape separator(sf::Vector2f(2.f, static_cast<float>(ROWS * TILE_SIZE)));
    separator.setPosition(static_cast<float>(COLS * TILE_SIZE), 0.f);
    separator.setFillColor(sf::Color(90, 90, 90));
    window.draw(separator);

    const sf::Font& font = m_context.getResources().getFont();
    float panelX = COLS * TILE_SIZE + 20.f;

    drawText(window, font, "Счёт", panelX, 20.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, std::to_string(m_facade.getScore()), panelX, 40.f, 24);

    drawText(window, font, "Уровень " + std::to_string(m_facade.getLevel()) + " / " + std::to_string(m_facade.getLevelCount()),
        panelX, 88.f, 16, sf::Color(180, 180, 180));
    drawText(window, font, "Сложность: " + std::string(difficultyName(m_facade.getDifficulty())), panelX, 108.f, 16);

    drawBrickLegend(window);
    drawBonusLegend(window);

    if (m_paused) {
        drawPauseOverlay(window);
    }
}

void PlayingState::drawBrickLegend(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();
    float panelX = COLS * TILE_SIZE + 20.f;

    const LegendEntry entries[] = {
        { BrickColors::DurableFresh, "Крепкий" },
        { BrickColors::Glass, "Стекло" },
        { BrickColors::Indestructible, "Неразрушимый" },
    };
    drawLegend(window, font, panelX, "Блоки", 150.f, entries, sizeof(entries) / sizeof(entries[0]));
}

void PlayingState::drawBonusLegend(sf::RenderWindow& window)
{
    const sf::Font& font = m_context.getResources().getFont();
    float panelX = COLS * TILE_SIZE + 20.f;

    const LegendEntry entries[] = {
        { PowerUpColors::Fireball, "Огненный мяч" },
        { PowerUpColors::FragileBlocks, "Хрупкие блоки" },
        { PowerUpColors::WidePaddle, "Широкая платформа" },
        { PowerUpColors::RemoveIndestructible, "Динамит" },
    };
    drawLegend(window, font, panelX, "Бонусы", 260.f, entries, sizeof(entries) / sizeof(entries[0]));
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

    drawText(window, font, "ПАУЗА", x, 200.f, 36);

    for (int i = 0; i < PAUSE_ITEM_COUNT; ++i) {
        PauseItem item = static_cast<PauseItem>(i);
        bool selected = (item == m_pauseSelected);
        std::string text = (selected ? "> " : "  ") + pauseItemLabel(item);
        float y = PAUSE_ITEM_START_Y + i * PAUSE_ITEM_SPACING;
        drawText(window, font, text, x, y, 24, selected ? sf::Color::Yellow : sf::Color::White);
    }

    float messageY = PAUSE_ITEM_START_Y + PAUSE_ITEM_COUNT * PAUSE_ITEM_SPACING + 20.f;
    if (!m_pauseMessage.empty()) {
        drawText(window, font, m_pauseMessage, x, messageY, 16, sf::Color(120, 220, 120));
        messageY += 24.f;
    }

    drawText(window, font, "ПКМ — продолжить игру", x, messageY, 16, sf::Color(180, 180, 180));
}

std::unique_ptr<IState> PlayingState::nextState()
{
    if (m_levelComplete) {
        if (m_facade.getLevel() < m_facade.getLevelCount()) {
            return std::make_unique<PlayingState>(m_context, m_facade.getDifficulty(), m_facade.getLevel() + 1, m_facade.getScore());
        }
        return std::make_unique<WinState>(m_context, m_facade.getScore());
    }
    if (m_gameOver) {
        return std::make_unique<GameOverState>(m_context, m_facade.getScore());
    }
    if (m_quitToMenuRequested) {
        return std::make_unique<MenuState>(m_context);
    }
    if (m_openSettingsRequested) {
        m_openSettingsRequested = false;

        // Не захватываем this/m_context в лямбде "назад" — к моменту, когда её вызовут, этот PlayingState уже будет уничтожен StateManager.
        GameContext& context = m_context;
        GameMemento memento = m_facade.createMemento();
        std::vector<BonusManager::ActiveBonusSnapshot> activeBonuses = m_facade.snapshotActiveBonuses();

        return std::make_unique<SettingsState>(m_context, m_context.getResources().getGameplayMusic(),
            [&context, memento, activeBonuses]() {
                return PlayingState::fromMemento(context, memento, true, activeBonuses);
            });
    }
    return nullptr;
}
