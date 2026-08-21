#pragma once
#include "GameState.h"
#include "IGameObserver.h"
#include <string>

// Паттерн "Фасад".
class GameFacade : private IGameObserver {
public:
    GameFacade(int difficulty, int level, int brickRows, int levelCount, int initialScore = 0);

    void update(sf::Time dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
    void requestPaddleMouseResync();

    bool isBallLost() const;
    bool isLevelComplete() const;

    int getScore() const;
    int getLevel() const;
    int getDifficulty() const;
    int getLevelCount() const;

    // Хранитель
    GameMemento createMemento() const;

    // Хранитель
    void restore(const GameMemento& memento);

    // Более лёгкий снимок активных бонусов, для переноса в памяти.
    std::vector<BonusManager::ActiveBonusSnapshot> snapshotActiveBonuses() const;
    void restoreActiveBonuses(const std::vector<BonusManager::ActiveBonusSnapshot>& snapshot);

    // Хранитель
    bool saveToFile(const std::string& filePath) const;

    // Хранитель
    bool loadFromFile(const std::string& filePath);

private:
    void onBrickDestroyed(int scoreValue) override;
    void onLevelComplete() override;

    GameState m_gameState;
    int m_level;
    int m_difficulty;
    int m_score;
    int m_levelCount;
    bool m_levelComplete = false;
};
