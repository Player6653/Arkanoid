#pragma once
#include <SFML/System/Time.hpp>
#include <map>
#include <memory>
#include <vector>
#include "BonusType.h"
#include "IBonusCommand.h"

class Ball;
class Paddle;
class GameState;

// Инициатор для команд-бонусов.
class BonusManager {
public:
    // Снимок одного активного бонуса.
    struct ActiveBonusSnapshot {
        BonusType type;
        sf::Time remaining;
    };

    BonusManager(Ball& ball, Paddle& paddle, GameState& gameState);

    // Активирует бонус. Если такой бонус уже активен — просто продлевает его действие.
    void activate(BonusType type);

    void update(sf::Time dt);

    // Сбрасывает учёт активных бонусов без вызова undo() — используется при восстановлении сохранения, когда объекты уже приведены к обычному состоянию напрямую.
    void clear();

    // Снимок всех активных сейчас бонусов (для переноса в памяти).
    std::vector<ActiveBonusSnapshot> snapshotActive() const;

    // Заново активирует каждый бонус из снимка (через activate(), т.е. с полноценным execute()) и сразу выставляет ему точное оставшееся время из снимка.
    void restoreActive(const std::vector<ActiveBonusSnapshot>& snapshot);

private:
    struct ActiveBonus {
        std::unique_ptr<IBonusCommand> command;
        sf::Time remaining;
    };

    Ball& m_ball;
    Paddle& m_paddle;
    GameState& m_gameState;
    std::map<BonusType, ActiveBonus> m_active;

    static sf::Time durationFor(BonusType type);
    std::unique_ptr<IBonusCommand> createCommand(BonusType type);
};
