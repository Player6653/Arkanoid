#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Brick.h"
#include "BrickFactory.h"
#include "GameMemento.h"
#include "PowerUp.h"
#include "BonusManager.h"

const short ROWS = 20;
const short COLS = 24;
const short TILE_SIZE = 30; // Размер одной клеточки поля.
const short UI_PANEL_WIDTH = 200; // Место справа от поля под счёт/сложность.

class Paddle;
class Ball;
class IGameObserver;

// Владеет платформой, шариком, блоками и бонусами.
class GameState {
public:
    GameState(int difficulty, int brickRows, int level);
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

    // Наблюдатель подписка на события "блок разрушен" / "уровень пройден".
    void addObserver(IGameObserver* observer);

    // Бонус "хрупкие блоки" пока активен, все разрушимые блоки бьются с одного удара.
    void setFragileBricksMode(bool active);

    // Бонус "Динамит" убирает с поля один случайный неразрушимый блок, если такие есть.
    void removeRandomIndestructibleBrick();

    // Хранитель (Memento) снимок текущего состояния и восстановление из него.
    GameMemento createMemento(int level, int difficulty, int score, int levelCount) const;
    void restore(const GameMemento& memento);

    // Более лёгкий снимок только активных бонусов.
    std::vector<BonusManager::ActiveBonusSnapshot> snapshotActiveBonuses() const;
    void restoreActiveBonuses(const std::vector<BonusManager::ActiveBonusSnapshot>& snapshot);

private:
    Paddle* m_paddle;
    Ball* m_ball;
    int m_brickRows;

    // Указатели на базовый класс.
    std::vector<std::unique_ptr<Brick>> m_bricks;
    std::vector<std::unique_ptr<PowerUp>> m_powerUps;
    std::unique_ptr<BonusManager> m_bonusManager;

    BrickFactory m_brickFactory;
    std::mt19937 m_rng;

    std::vector<IGameObserver*> m_observers;

    bool m_paddleMouseResyncPending = false;
    bool m_levelCompleteNotified = false;
    bool m_fragileBricksMode = false;

    // true, если блоков на поле больше не осталось (победа).
    bool areAllBricksDestroyed() const;

    void spawnBricks();
    void handleBrickCollisions();
    void updatePowerUps(sf::Time dt);
    void maybeSpawnPowerUp(sf::FloatRect brickBounds);

    void notifyBrickDestroyed(int scoreValue);
    void notifyLevelComplete();
};
