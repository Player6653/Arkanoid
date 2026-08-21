#include "GameState.h"
#include "Paddle.h"
#include "Ball.h"
#include "IGameObserver.h"
#include "BonusManager.h"

namespace {
    const int BRICK_COLS = 12; // число рядов теперь настраивается игроком.
    const float BRICK_GAP = 4.f;
    const float BRICK_TOP_MARGIN = 40.f;
    const float BRICK_HEIGHT = 14.f;

    // Шанс, что с разрушенного блока упадёт бонус.
    const float POWERUP_DROP_CHANCE = 0.10f;

    // "Динамит" (RemoveIndestructible) — единственный бонус, убирающий неразрушимые блоки.
    const BonusType POWERUP_TYPES[] = {
        BonusType::Fireball, BonusType::FragileBlocks, BonusType::WidePaddle,
        BonusType::RemoveIndestructible, BonusType::RemoveIndestructible
    };
}

GameState::GameState(int difficulty, int brickRows, int level)
    : m_paddle(nullptr)
    , m_ball(nullptr)
    , m_brickRows(brickRows)
    , m_brickFactory(level)
    , m_rng(std::random_device{}())
{
    // Единственное место, где вызывается new/delete на Paddle/Ball. try/catch — на случай, если второй new бросит исключение.
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

        m_bonusManager = std::make_unique<BonusManager>(*m_ball, *m_paddle, *this);

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
    // BonusManager (если ещё жив) не трогает Ball/Paddle в своём деструкторе (undo() не вызывается), поэтому порядок уничтожения относительно delete m_paddle/m_ball ниже не важен.
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
            sf::Vector2f position(x, y);
            sf::Vector2f size(brickWidth, BRICK_HEIGHT);

            m_bricks.push_back(m_brickFactory.createRandomBrick(position, size, m_rng));
        }
    }
}

void GameState::handleBrickCollisions()
{
    sf::FloatRect ballBounds = m_ball->getBounds();
    const IBallBehavior& behavior = m_ball->getBehavior();

    for (std::size_t i = 0; i < m_bricks.size(); ++i) {
        Brick* brick = m_bricks[i].get();

        if (ballBounds.intersects(brick->getBounds())) {
            bool brickIsDestructible = brick->countsTowardWin();
            bool shouldBounce = behavior.resolveBounce(brick->shouldBounceBall(), brickIsDestructible);
            bool forceInstantBreak = brickIsDestructible && (m_fragileBricksMode || behavior.forcesInstantBreak());

            if (forceInstantBreak) {
                brick->forceDestroy();
            }
            else {
                brick->onHit();
            }

            if (shouldBounce) {
                m_ball->bounceOffBrick(brick->getBounds());
            }

            if (brick->isDestroyed()) {
                notifyBrickDestroyed(brick->getScoreValue());
                maybeSpawnPowerUp(brick->getBounds());
                m_bricks.erase(m_bricks.begin() + i);
            }
            break; // за один кадр обрабатываем не больше одного блока.
        }
    }
}

void GameState::maybeSpawnPowerUp(sf::FloatRect brickBounds)
{
    std::uniform_real_distribution<float> chanceRoll(0.f, 1.f);
    if (chanceRoll(m_rng) > POWERUP_DROP_CHANCE) {
        return;
    }

    const int powerUpTypeCount = sizeof(POWERUP_TYPES) / sizeof(POWERUP_TYPES[0]);
    std::uniform_int_distribution<int> typeRoll(0, powerUpTypeCount - 1);
    BonusType type = POWERUP_TYPES[typeRoll(m_rng)];

    sf::Vector2f center(brickBounds.left + brickBounds.width / 2.f, brickBounds.top + brickBounds.height / 2.f);
    m_powerUps.push_back(std::make_unique<PowerUp>(center, type));
}

void GameState::updatePowerUps(sf::Time dt)
{
    const float fieldHeight = static_cast<float>(ROWS * TILE_SIZE);
    sf::FloatRect paddleBounds = m_paddle->getBounds();

    for (std::size_t i = 0; i < m_powerUps.size(); ) {
        PowerUp* powerUp = m_powerUps[i].get();
        powerUp->update(dt);

        if (powerUp->getBounds().intersects(paddleBounds)) {
            m_bonusManager->activate(powerUp->getType());
            m_powerUps.erase(m_powerUps.begin() + i);
        }
        else if (powerUp->isBelow(fieldHeight)) {
            m_powerUps.erase(m_powerUps.begin() + i);
        }
        else {
            ++i;
        }
    }
}

void GameState::requestPaddleMouseResync()
{
    m_paddleMouseResyncPending = true;
}

void GameState::update(sf::Time dt, const sf::RenderWindow& window)
{
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
    updatePowerUps(dt);
    m_bonusManager->update(dt);

    if (!m_levelCompleteNotified && areAllBricksDestroyed()) {
        m_levelCompleteNotified = true;
        notifyLevelComplete();
    }
}

void GameState::draw(sf::RenderWindow& window) const
{
    // Вызов через указатель на базовый Brick.
    for (const std::unique_ptr<Brick>& brick : m_bricks) {
        brick->draw(window);
    }
    for (const std::unique_ptr<PowerUp>& powerUp : m_powerUps) {
        powerUp->draw(window);
    }
    m_paddle->draw(window);
    m_ball->draw(window);
}

bool GameState::isBallLost() const
{
    return m_ball->isBelow(static_cast<float>(ROWS * TILE_SIZE));
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

void GameState::addObserver(IGameObserver* observer)
{
    m_observers.push_back(observer);
}

void GameState::notifyBrickDestroyed(int scoreValue)
{
    for (IGameObserver* observer : m_observers) {
        observer->onBrickDestroyed(scoreValue);
    }
}

void GameState::notifyLevelComplete()
{
    for (IGameObserver* observer : m_observers) {
        observer->onLevelComplete();
    }
}

void GameState::setFragileBricksMode(bool active)
{
    m_fragileBricksMode = active;
}

void GameState::removeRandomIndestructibleBrick()
{
    std::vector<std::size_t> indestructibleIndices;
    for (std::size_t i = 0; i < m_bricks.size(); ++i) {
        if (m_bricks[i]->getKind() == BrickKind::Indestructible) {
            indestructibleIndices.push_back(i);
        }
    }

    if (indestructibleIndices.empty()) {
        return;
    }

    std::uniform_int_distribution<std::size_t> pick(0, indestructibleIndices.size() - 1);
    m_bricks.erase(m_bricks.begin() + indestructibleIndices[pick(m_rng)]);
}

GameMemento GameState::createMemento(int level, int difficulty, int score, int levelCount) const
{
    GameMemento memento;
    memento.m_level = level;
    memento.m_difficulty = difficulty;
    memento.m_score = score;
    memento.m_levelCount = levelCount;
    memento.m_ballPosition = m_ball->getPosition();
    memento.m_ballVelocity = m_ball->getNormalizedVelocity();
    memento.m_paddleX = m_paddle->getNormalizedX();

    memento.m_bricks.reserve(m_bricks.size());
    for (const std::unique_ptr<Brick>& brick : m_bricks) {
        sf::FloatRect bounds = brick->getBounds();
        GameMemento::BrickSnapshot snapshot;
        snapshot.kind = brick->getKind();
        snapshot.x = bounds.left;
        snapshot.y = bounds.top;
        snapshot.width = bounds.width;
        snapshot.height = bounds.height;
        snapshot.hitsRemaining = brick->getHitsRemaining();
        memento.m_bricks.push_back(snapshot);
    }

    return memento;
}

void GameState::restore(const GameMemento& memento)
{
    m_ball->setPosition(memento.m_ballPosition);
    m_ball->setVelocity(memento.m_ballVelocity);
    m_ball->setBehavior(std::make_unique<IBallBehavior>());
    m_ball->clearTint();

    m_paddle->setState(std::make_unique<IPaddleState>());
    m_paddle->setX(memento.m_paddleX);

    m_fragileBricksMode = false;
    m_powerUps.clear();
    m_bonusManager->clear();

    m_bricks.clear();
    for (const GameMemento::BrickSnapshot& snapshot : memento.m_bricks) {
        sf::Vector2f position(snapshot.x, snapshot.y);
        sf::Vector2f size(snapshot.width, snapshot.height);
        m_bricks.push_back(m_brickFactory.createExactBrick(snapshot.kind, position, size, snapshot.hitsRemaining));
    }

    m_levelCompleteNotified = false;
}

std::vector<BonusManager::ActiveBonusSnapshot> GameState::snapshotActiveBonuses() const
{
    return m_bonusManager->snapshotActive();
}

void GameState::restoreActiveBonuses(const std::vector<BonusManager::ActiveBonusSnapshot>& snapshot)
{
    m_bonusManager->restoreActive(snapshot);
}
