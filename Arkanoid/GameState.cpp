#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"

GameState::GameState(int difficulty)
{
    const float paddleWidth = 70.f;
    const float paddleHeight = 12.f;
    const float paddleX = (COLS * TILE_SIZE - paddleWidth) / 2.f;
    const float paddleY = ROWS * TILE_SIZE - paddleHeight - 10.f;

    // Единственное место, где вызывается new/delete
    m_paddle = new Paddle(sf::Vector2f(paddleX, paddleY), sf::Vector2f(paddleWidth, paddleHeight));

    const float ballSpeed = 200.f + difficulty * 60.f; // чем выше сложность, тем быстрее шарик
    const sf::Vector2f ballStart(COLS * TILE_SIZE / 2.f, paddleY - 10.f);
    const sf::Vector2f ballVelocity(ballSpeed * 0.6f, -ballSpeed);
    m_ball = new Ball(ballStart, ballVelocity);
}

GameState::~GameState()
{
    delete m_paddle;
    delete m_ball;
}

void GameState::update(sf::Time dt, const sf::RenderWindow& window)
{
    m_paddle->update(dt, window);
    m_ball->update(dt);
    m_ball->bounceOffWalls(static_cast<float>(COLS * TILE_SIZE));

    if (m_ball->getBounds().intersects(m_paddle->getBounds())) {
        m_ball->bounceOffPaddle();
    }
}

void GameState::draw(sf::RenderWindow& window) const
{
    m_paddle->draw(window);
    m_ball->draw(window);
}

bool GameState::isBallLost() const
{
    return m_ball->isBelow(static_cast<float>(ROWS * TILE_SIZE));
}
