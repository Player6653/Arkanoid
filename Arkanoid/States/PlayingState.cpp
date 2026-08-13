#include "PlayingState.h"
#include "GameOverState.h"
#include "MenuState.h"
#include "../ResourceManager.h"
#include "../UiText.h"
#include "../Audio.h"
#include "../Settings.h"
#include <cstdlib>
#include <ctime>
#include <string>

namespace {
    TetrominoType randomTetrominoType()
    {
        return static_cast<TetrominoType>(std::rand() % 7);
    }
}

PlayingState::PlayingState(GameContext& context, int difficulty)
    : m_context(context)
    , m_current(TetrominoType::T) // временная форма
    , m_next(TetrominoType::T) // временная форма
    , m_hold(TetrominoType::T) // заглушка
{
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    // Стартовые скорость и уровень.
    if (difficulty <= 1) {
        m_fallInterval = sf::seconds(0.8f);
    }
    else if (difficulty == 2) {
        m_fallInterval = sf::seconds(0.5f);
    }
    else {
        m_fallInterval = sf::seconds(0.3f);
    }
    m_level = difficulty;

    m_rotateSound.setBuffer(m_context.resources.getRotateBuffer());
    m_lineClearSound.setBuffer(m_context.resources.getLineClearBuffer());
    m_swapSound.setBuffer(m_context.resources.getSwapBuffer());
    m_pauseMoveSound.setBuffer(m_context.resources.getRotateBuffer());
    m_pauseToggleSound.setBuffer(m_context.resources.getSwapBuffer());

    switchMusic(m_context.settings, m_context.resources.getMenuMusic(), m_context.resources.getGameplayMusic());

    m_next = Tetromino(randomTetrominoType());
    spawnPiece();
}

bool PlayingState::hasCollision(const Tetromino& piece) const
{
    for (const sf::Vector2i& cell : piece.getCells()) {
        if (m_board.isCollision(cell.x, cell.y)) {
            return true;
        }
    }
    return false;
}

bool PlayingState::hasCollision() const
{
    return hasCollision(m_current);
}

void PlayingState::tryMove(int dx, int dy)
{
    m_current.moveBy(dx, dy);
    if (hasCollision()) {
        m_current.moveBy(-dx, -dy); 
    }
}

void PlayingState::tryRotate()
{
    int previousRotation = m_current.getRotation();
    m_current.rotate();

    if (!hasCollision()) {
        playSfx(m_context.settings, m_rotateSound);
        return;
    }

    // Вектор поворота
    const sf::Vector2i kicks[] = {
        { -1, 0 }, { 1, 0 }, { -2, 0 }, { 2, 0 }, { 0, -1 }
    };

    for (const sf::Vector2i& kick : kicks) {
        m_current.moveBy(kick.x, kick.y);
        if (!hasCollision()) {
            playSfx(m_context.settings, m_rotateSound);
            return;
        }
        m_current.moveBy(-kick.x, -kick.y);
    }

    // Отмена поворот целиком.
    m_current.setRotation(previousRotation);
}

void PlayingState::lockPiece()
{
    for (const sf::Vector2i& cell : m_current.getCells()) {
        m_board.setFilled(cell.x, cell.y);
    }
}

void PlayingState::lockCurrentAndSpawnNext()
{
    lockPiece();

    int lines = m_board.clearFullLines();
    m_score += scoreForLines(lines);
    if (lines > 0) {
        playSfx(m_context.settings, m_lineClearSound);
    }

    m_canHold = true; // фигура зафиксировалась.

    spawnPiece();

    if (hasCollision()) {
        m_gameOver = true;
    }
}

void PlayingState::spawnPiece()
{
    m_current = m_next;
    m_next = Tetromino(randomTetrominoType());
}

Tetromino PlayingState::computeGhost() const
{
    Tetromino ghost = m_current;
    while (!hasCollision(ghost)) {
        ghost.moveBy(0, 1);
    }
    ghost.moveBy(0, -1); // откат на одну клетку назад.
    return ghost;
}

void PlayingState::hardDrop()
{
    m_current = computeGhost();
    lockCurrentAndSpawnNext();
}

void PlayingState::holdPiece()
{
    if (!m_canHold) {
        return;
    }
    m_canHold = false;

    if (!m_hasHold) {
        m_hold = m_current;
        m_hasHold = true;
        spawnPiece();
    }
    else {
        TetrominoType heldType = m_hold.getType();
        m_hold = Tetromino(m_current.getType());
        m_current = Tetromino(heldType);
    }

    playSfx(m_context.settings, m_swapSound);

    if (hasCollision()) {
        m_gameOver = true;
    }
}

int PlayingState::scoreForLines(int lines) const
{
    switch (lines) {
        case 1: return 100 * m_level;
        case 2: return 300 * m_level;
        case 3: return 500 * m_level;
        case 4: return 800 * m_level; 
        default: return 0;
    }
}

void PlayingState::setPaused(bool paused)
{
    m_paused = paused;

    if (m_paused) {
        m_context.resources.getGameplayMusic().pause();
    }
    else if (m_context.settings.musicOn) {
        playIfNotPlaying(m_context.resources.getGameplayMusic());
    }
}

void PlayingState::movePauseSelection(int direction)
{
    int index = static_cast<int>(m_pauseSelected);
    index = (index + direction + PAUSE_ITEM_COUNT) % PAUSE_ITEM_COUNT;
    m_pauseSelected = static_cast<PauseItem>(index);

    playSfx(m_context.settings, m_pauseMoveSound);
}

void PlayingState::activatePauseSelection()
{
    switch (m_pauseSelected) {
        case PauseItem::Resume:
            setPaused(false);
            break;

        case PauseItem::ToggleSound:
            m_context.settings.soundOn = !m_context.settings.soundOn;
            playSfx(m_context.settings, m_pauseToggleSound);
            break;

        case PauseItem::ToggleMusic:
            m_context.settings.musicOn = !m_context.settings.musicOn;
            if (m_context.settings.musicOn) {
                playIfNotPlaying(m_context.resources.getGameplayMusic());
            }
            else {
                m_context.resources.getGameplayMusic().pause();
            }
            playSfx(m_context.settings, m_pauseToggleSound);
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
            return std::string("Звук: ") + (m_context.settings.soundOn ? "ВКЛ" : "ВЫКЛ");
        case PauseItem::ToggleMusic:
            return std::string("Музыка: ") + (m_context.settings.musicOn ? "ВКЛ" : "ВЫКЛ");
        case PauseItem::QuitToMenu:
            return "Выйти в меню";
    }
    return "";
}

void PlayingState::handleGameplayInput(const sf::Event& event)
{
    if (event.type != sf::Event::KeyPressed) {
        return;
    }

    switch (event.key.code) {
        case sf::Keyboard::Left:
        case sf::Keyboard::A:
            tryMove(-1, 0);
            break;

        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            tryMove(1, 0);
            break;

        case sf::Keyboard::Down:
        case sf::Keyboard::S:
            tryMove(0, 1);
            break;

        case sf::Keyboard::Up:
        case sf::Keyboard::W:
            tryRotate();
            break;

        case sf::Keyboard::Space:
            // Реагируем только на первое нажатие.
            if (!m_hardDropKeyHeld) {
                m_hardDropKeyHeld = true;
                hardDrop();
            }
            break;

        case sf::Keyboard::C:
            holdPiece();
            break;

        default:
            break;
    }
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
            if (!m_hardDropKeyHeld) {
                m_hardDropKeyHeld = true;
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
            m_hardDropKeyHeld = false;
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
    else {
        handleGameplayInput(event);
    }
}

void PlayingState::update(sf::Time dt)
{
    if (m_paused) {
        return;
    }

    m_fallTimer += dt;
    if (m_fallTimer < m_fallInterval) {
        return;
    }
    m_fallTimer = sf::Time::Zero;

    m_current.moveBy(0, 1);
    if (hasCollision()) {
        m_current.moveBy(0, -1); // Падать больше некуда.
        lockCurrentAndSpawnNext();
    }
}

void PlayingState::draw(sf::RenderWindow& window)
{
    m_board.draw(window);

    // полупрозрачный силуэт там, куда упадёт текущая фигура.
    if (!m_paused) {
        Tetromino ghost = computeGhost();
        sf::Color ghostColor = sf::Color::Cyan;
        ghostColor.a = 70; // полупрозрачность
        for (const sf::Vector2i& cell : ghost.getCells()) {
            drawTile(window, cell.x, cell.y, ghostColor);
        }
    }

    for (const sf::Vector2i& cell : m_current.getCells()) {
        drawTile(window, cell.x, cell.y, sf::Color::Cyan);
    }

    sf::RectangleShape separator(sf::Vector2f(2.f, static_cast<float>(ROWS * TILE_SIZE)));
    separator.setPosition(static_cast<float>(COLS * TILE_SIZE), 0.f);
    separator.setFillColor(sf::Color(90, 90, 90));
    window.draw(separator);

    const sf::Font& font = m_context.resources.getFont();
    float panelX = COLS * TILE_SIZE + 20.f;

    drawText(window, font, "Счёт", panelX, 40.f, 18, sf::Color(180, 180, 180));
    drawText(window, font, std::to_string(m_score), panelX, 62.f, 28);

    drawText(window, font, "Сложность", panelX, 130.f, 18, sf::Color(180, 180, 180));
    drawText(window, font, difficultyName(m_level), panelX, 152.f, 22);

    drawPiecePreview(window, font, "Следующая", m_next.getType(), panelX, 220.f);

    drawText(window, font, "Карман (C)", panelX, 410.f, 18, sf::Color(180, 180, 180));
    if (m_hasHold) {
        drawPieceGraphic(window, font, m_hold.getType(), panelX, 430.f);
    }
    else {
        drawText(window, font, "-", panelX, 430.f, 24);
    }

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

    const sf::Font& font = m_context.resources.getFont();
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

void PlayingState::drawPieceGraphic(sf::RenderWindow& window, const sf::Font& font,
    TetrominoType type, float x, float y) const
{
    drawText(window, font, tetrominoTypeName(type), x, y, 24);

    // Графическое превью формы фигуры — буквы одной не всегда достаточно,
    // чтобы сразу понять, как фигура выглядит.
    for (const sf::Vector2i& cell : Tetromino::previewCells(type)) {
        drawTile(window, cell.x, cell.y, sf::Color::Cyan, sf::Vector2f(x, y + 35.f));
    }
}

void PlayingState::drawPiecePreview(sf::RenderWindow& window, const sf::Font& font,
    const std::string& label, TetrominoType type, float x, float y) const
{
    drawText(window, font, label, x, y, 18, sf::Color(180, 180, 180));
    drawPieceGraphic(window, font, type, x, y + 20.f);
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
