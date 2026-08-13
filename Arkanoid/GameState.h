#pragma once
#include <SFML/Graphics.hpp>

const short ROWS = 20;
const short COLS = 10;
const short TILE_SIZE = 30; // Размер одной клеточки поля в пикселях
const short UI_PANEL_WIDTH = 200; // Место справа от поля под счёт/сложность

class Paddle;
class Ball;

// Владеет платформой и шариком.
class GameState {
public:
    explicit GameState(int difficulty);
    ~GameState();

    // Копирование запрещено
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    void update(sf::Time dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    // true, если шарик целиком улетел ниже платформы.
    bool isBallLost() const;

private:
    Paddle* m_paddle;
    Ball* m_ball;
};
