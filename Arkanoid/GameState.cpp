#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"

namespace {
    const int BRICK_COLS = 12; // число рядов теперь настраивается игроком, столбцов всегда 12.
    const float BRICK_GAP = 4.f;
    const float BRICK_TOP_MARGIN = 40.f;
    const float BRICK_HEIGHT = 14.f;

    sf::Color brickColorForRow(int row)
    {
        switch (row % 4) {
            case 0: return sf::Color(220, 60, 60);   // красный
            case 1: return sf::Color(230, 160, 40);  // оранжевый
            case 2: return sf::Color(80, 180, 90);   // зелёный
            default: return sf::Color(70, 130, 220); // синий
        }
    }
}

GameState::GameState(int difficulty, int brickRows)
    : m_paddle(nullptr)
    , m_ball(nullptr)
    , m_brickRows(brickRows)
{
    // Единственное место, где вызывается new/delete. try/catch — на случай, если че второй new бросит исключение.
    try {
        const float paddleWidth = 100.f; 
        const float paddleHeight = 12.f;
        const float paddleX = (COLS * TILE_SIZE - paddleWidth) / 2.f;
        const float paddleY = ROWS * TILE_SIZE - paddleHeight - 10.f;

        m_paddle = new Paddle(sf::Vector2f(paddleX, paddleY), sf::Vector2f(paddleWidth, paddleHeight));

        const float ballSpeed = 200.f + difficulty * 60.f; // чем выше сложность, тем быстрее шарик.
        const sf::Vector2f ballStart(COLS * TILE_SIZE / 2.f, paddleY - 10.f);
        const sf::Vector2f ballVelocity(ballSpeed * 0.6f, -ballSpeed);
        m_ball = new Ball(ballStart, ballVelocity);

        spawnBricks();
    }
    catch (...) {
        delete m_paddle;
        delete m_ball;
        throw;
    }
}

GameState::~GameState()
{
    delete m_paddle;
    delete m_ball;
}

void GameState::spawnBricks()
{
    const float fieldWidth = static_cast<float>(COLS * TILE_SIZE);
    const float brickWidth = (fieldWidth - BRICK_GAP * (BRICK_COLS + 1)) / BRICK_COLS;

    for (int row = 0; row < m_brickRows; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            float x = BRICK_GAP + col * (brickWidth + BRICK_GAP);
            float y = BRICK_TOP_MARGIN + row * (BRICK_HEIGHT + BRICK_GAP);
            m_bricks.push_back(Brick(sf::Vector2f(x, y), sf::Vector2f(brickWidth, BRICK_HEIGHT), brickColorForRow(row)));
        }
    }
}

void GameState::handleBrickCollisions()
{
    sf::FloatRect ballBounds = m_ball->getBounds();

    for (std::size_t i = 0; i < m_bricks.size(); ++i) {
        if (ballBounds.intersects(m_bricks[i].getBounds())) {
            m_ball->bounceOffBrick(m_bricks[i].getBounds());
            m_bricks.erase(m_bricks.begin() + i);
            m_brickDestroyedThisUpdate = true;
            break; // за один кадр разбиваем не больше одного блока.
        }
    }
}

void GameState::requestPaddleMouseResync()
{
    m_paddleMouseResyncPending = true;
}

void GameState::update(sf::Time dt, const sf::RenderWindow& window)
{
    m_brickDestroyedThisUpdate = false;

    if (m_paddleMouseResyncPending) {
        m_paddle->resyncMouse(window);
        m_paddleMouseResyncPending = false;
    }

    m_paddle->update(dt, window);
    m_ball->update(dt);
    m_ball->bounceOffWalls(static_cast<float>(COLS * TILE_SIZE));

    if (m_ball->getBounds().intersects(m_paddle->getBounds())) {
        m_ball->bounceOffPaddle();
    }

    handleBrickCollisions();
}

void GameState::draw(sf::RenderWindow& window) const
{
    // Через ссылку на базовый GameObject.
    for (const GameObject& brick : m_bricks) {
        brick.draw(window);
    }
    m_paddle->draw(window);
    m_ball->draw(window);
}

bool GameState::isBallLost() const
{
    return m_ball->isBelow(static_cast<float>(ROWS * TILE_SIZE));
}

bool GameState::wasBrickDestroyed() const
{
    return m_brickDestroyedThisUpdate;
}

bool GameState::areAllBricksDestroyed() const
{
    return m_bricks.empty();
}
