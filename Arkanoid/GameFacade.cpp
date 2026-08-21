#include "GameFacade.h"
#include "GameMemento.h"
#include "Settings.h"
#include <cmath>

GameFacade::GameFacade(int difficulty, int level, int brickRows, int levelCount, int initialScore)
    : m_gameState(difficulty, brickRows, level)
    , m_level(level)
    , m_difficulty(difficulty)
    , m_score(initialScore)
    , m_levelCount(levelCount)
{
    m_gameState.addObserver(this);
}

void GameFacade::update(sf::Time dt, const sf::RenderWindow& window)
{
    m_gameState.update(dt, window);
}

void GameFacade::draw(sf::RenderWindow& window) const
{
    m_gameState.draw(window);
}

void GameFacade::requestPaddleMouseResync()
{
    m_gameState.requestPaddleMouseResync();
}

bool GameFacade::isBallLost() const
{
    return m_gameState.isBallLost();
}

bool GameFacade::isLevelComplete() const
{
    return m_levelComplete;
}

int GameFacade::getScore() const
{
    return m_score;
}

int GameFacade::getLevel() const
{
    return m_level;
}

int GameFacade::getDifficulty() const
{
    return m_difficulty;
}

int GameFacade::getLevelCount() const
{
    return m_levelCount;
}

void GameFacade::onBrickDestroyed(int scoreValue)
{
    // Лёгкая x1, средняя x1.25, сложная x1.5 — округляем, а не отбрасываем.
    m_score += static_cast<int>(std::round(scoreValue * difficultyScoreMultiplier(m_difficulty)));
}

void GameFacade::onLevelComplete()
{
    m_levelComplete = true;
}

GameMemento GameFacade::createMemento() const
{
    return m_gameState.createMemento(m_level, m_difficulty, m_score, m_levelCount);
}

void GameFacade::restore(const GameMemento& memento)
{
    m_gameState.restore(memento);
    m_level = memento.getLevel();
    m_difficulty = memento.getDifficulty();
    m_score = memento.getScore();
    m_levelCount = memento.getLevelCount();
    m_levelComplete = false;
}

std::vector<BonusManager::ActiveBonusSnapshot> GameFacade::snapshotActiveBonuses() const
{
    return m_gameState.snapshotActiveBonuses();
}

void GameFacade::restoreActiveBonuses(const std::vector<BonusManager::ActiveBonusSnapshot>& snapshot)
{
    m_gameState.restoreActiveBonuses(snapshot);
}

bool GameFacade::saveToFile(const std::string& filePath) const
{
    return createMemento().save(filePath);
}

bool GameFacade::loadFromFile(const std::string& filePath)
{
    GameMemento memento;
    if (!GameMemento::load(filePath, memento)) {
        return false;
    }

    restore(memento);
    return true;
}
