#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"
#include "DurableBrick.h"
#include "GlassBrick.h"
#include "IndestructibleBrick.h"
#include "BrickColors.h"
#include <random>

namespace {
    const int BRICK_COLS = 12; // число рядов теперь настраивается игроком, столбцов всегда 12.
    const float BRICK_GAP = 4.f;
    const float BRICK_TOP_MARGIN = 40.f;
    const float BRICK_HEIGHT = 14.f;

    // Доля клеток под каждый спец-тип (суммарно должны 1.0).
    const float INDESTRUCTIBLE_CHANCE = 0.05f;
    const float GLASS_CHANCE = 0.08f;
    const float DURABLE_CHANCE = 0.15f;
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

    // Свой генератор на каждый вызов spawnBricks(), поэтому раскладка спец-блоков отличается
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_real_distribution<float> roll(0.f, 1.f);

    for (int row = 0; row < m_brickRows; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            float x = BRICK_GAP + col * (brickWidth + BRICK_GAP);
            float y = BRICK_TOP_MARGIN + row * (BRICK_HEIGHT + BRICK_GAP);
            sf::Vector2f position(x, y);
            sf::Vector2f size(brickWidth, BRICK_HEIGHT);

            float r = roll(rng);
            if (r < INDESTRUCTIBLE_CHANCE) {
                m_bricks.push_back(std::make_unique<IndestructibleBrick>(position, size));
            }
            else if (r < INDESTRUCTIBLE_CHANCE + GLASS_CHANCE) {
                m_bricks.push_back(std::make_unique<GlassBrick>(position, size));
            }
            else if (r < INDESTRUCTIBLE_CHANCE + GLASS_CHANCE + DURABLE_CHANCE) {
                m_bricks.push_back(std::make_unique<DurableBrick>(position, size, 3));
            }
            else {
                m_bricks.push_back(std::make_unique<Brick>(position, size, BrickColors::Normal));
            }
        }
    }
}

void GameState::handleBrickCollisions()
{
    sf::FloatRect ballBounds = m_ball->getBounds();

    for (std::size_t i = 0; i < m_bricks.size(); ++i) {
        Brick* brick = m_bricks[i].get();

        if (ballBounds.intersects(brick->getBounds())) {
            // Узнаём про отскок до onHit().
            bool shouldBounce = brick->shouldBounceBall();
            brick->onHit();

            if (shouldBounce) {
                m_ball->bounceOffBrick(brick->getBounds());
            }

            if (brick->isDestroyed()) {
                m_bricks.erase(m_bricks.begin() + i);
                m_brickDestroyedThisUpdate = true;
            }
            break; // за один кадр обрабатываем не больше одного блока.
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
    // Вызов через указатель на базовый Brick.
    for (const std::unique_ptr<Brick>& brick : m_bricks) {
        brick->draw(window);
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
    for (const std::unique_ptr<Brick>& brick : m_bricks) {
        if (brick->countsTowardWin()) {
            return false;
        }
    }
    return true;
}
