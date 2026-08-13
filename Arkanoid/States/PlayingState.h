// States/PlayingState.h — экран игры
#pragma once
#include "../IState.h"
#include "../GameContext.h"
#include "../Board.h"
#include "../Tetromino.h"
#include <SFML/Audio.hpp>
#include <string>

// физика фигур
class PlayingState : public IState {
public:
    PlayingState(GameContext& context, int difficulty);

    void handleInput(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<IState> nextState() override;

private:
    GameContext& m_context;
    Board m_board;
    Tetromino m_current;
    Tetromino m_next;

    Tetromino m_hold;
    bool m_hasHold = false;
    bool m_canHold = true;

    sf::Sound m_rotateSound;
    sf::Sound m_lineClearSound;
    sf::Sound m_swapSound;

    int m_score = 0;
    int m_level = 1;
    sf::Time m_fallTimer;
    sf::Time m_fallInterval;

    bool m_gameOver = false;

    bool m_hardDropKeyHeld = false;
    bool m_escapeKeyHeld = false;
    bool m_pauseEnterKeyHeld = false;

    enum class PauseItem { Resume, ToggleSound, ToggleMusic, QuitToMenu };
    static const int PAUSE_ITEM_COUNT = 4;

    bool m_paused = false;
    PauseItem m_pauseSelected = PauseItem::Resume;
    bool m_quitToMenuRequested = false;

    sf::Sound m_pauseMoveSound;  
    sf::Sound m_pauseToggleSound;

    bool hasCollision(const Tetromino& piece) const;

    // Перегрузка без аргумента.
    bool hasCollision() const;

    // Пробует сдвинуть m_current на (dx, dy); если после сдвига коллизия то откатывает обратно.
    void tryMove(int dx, int dy);

    void tryRotate();

    // Записывает клетки m_current в Board
    void lockPiece();

    // Фиксирует m_current в Board
    void lockCurrentAndSpawnNext();

    void spawnPiece();

    Tetromino computeGhost() const;

    // Мгновенно роняет m_current вниз до упора.
    void hardDrop();

    // Свап фигуры.
    void holdPiece();

    // Очки.
    int scoreForLines(int lines) const;

    void handleGameplayInput(const sf::Event& event);

    void handlePausedInput(const sf::Event& event);

    void setPaused(bool paused);

    void movePauseSelection(int direction);
    void activatePauseSelection();
    std::string pauseItemLabel(PauseItem item) const;
    void drawPauseOverlay(sf::RenderWindow& window);

    void drawPieceGraphic(sf::RenderWindow& window, const sf::Font& font, TetrominoType type, float x, float y) const;

    void drawPiecePreview(sf::RenderWindow& window, const sf::Font& font,
        const std::string& label, TetrominoType type, float x, float y) const;
};
