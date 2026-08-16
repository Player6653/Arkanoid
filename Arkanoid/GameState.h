#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Brick.h"

const short ROWS = 20;
const short COLS = 24;
const short TILE_SIZE = 30; // Размер одной клеточки поля.
const short UI_PANEL_WIDTH = 200; // Место справа от поля под счёт/сложность.

class Paddle;
class Ball;

// Владеет платформой, шариком и блоками.
class GameState {
public:
    GameState(int difficulty, int brickRows);
    ~GameState();

    // Копирование запрещено.
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    void update(sf::Time dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    // Просит на следующем update() ресинхронизировать позицию мыши.
    void requestPaddleMouseResync();

    // true, если шарик целиком улетел ниже платформы.
    bool isBallLost() const;

    // true, если блок был разбит именно на последнем update() (для начисления очков).
    bool wasBrickDestroyed() const;

    // true, если блоков на поле больше не осталось (победа).
    bool areAllBricksDestroyed() const;

private:
    Paddle* m_paddle;
    Ball* m_ball;
    int m_brickRows;
    // Указатели на базовый класс 
    std::vector<std::unique_ptr<Brick>> m_bricks;
    bool m_brickDestroyedThisUpdate = false;
    bool m_paddleMouseResyncPending = false;

    void spawnBricks();
    void handleBrickCollisions();
};
