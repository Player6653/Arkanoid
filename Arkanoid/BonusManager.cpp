#include "BonusManager.h"
#include "Ball.h"
#include "Paddle.h"
#include "GameState.h"
#include "FireballBonusCommand.h"
#include "FragileBlocksBonusCommand.h"
#include "WidePaddleBonusCommand.h"
#include "RemoveIndestructibleBonusCommand.h"
#include <cassert>

BonusManager::BonusManager(Ball& ball, Paddle& paddle, GameState& gameState)
    : m_ball(ball)
    , m_paddle(paddle)
    , m_gameState(gameState)
{
}

void BonusManager::activate(BonusType type)
{
    auto it = m_active.find(type);
    if (it != m_active.end()) {
        // Бонус уже активен — просто продлеваем время действия.
        it->second.remaining = durationFor(type);
        return;
    }

    std::unique_ptr<IBonusCommand> command = createCommand(type);
    command->execute();

    if (command->isInstant()) {
        // Одноразовый эффект без длительности — ни таймер, ни undo() ему не нужны.
        return;
    }

    ActiveBonus bonus;
    bonus.remaining = durationFor(type);
    bonus.command = std::move(command);
    m_active.emplace(type, std::move(bonus));
}

void BonusManager::update(sf::Time dt)
{
    for (auto it = m_active.begin(); it != m_active.end(); ) {
        it->second.remaining -= dt;
        if (it->second.remaining <= sf::Time::Zero) {
            it->second.command->undo();
            it = m_active.erase(it);
        }
        else {
            ++it;
        }
    }
}

void BonusManager::clear()
{
    m_active.clear();
}

std::vector<BonusManager::ActiveBonusSnapshot> BonusManager::snapshotActive() const
{
    std::vector<ActiveBonusSnapshot> result;
    result.reserve(m_active.size());
    for (const auto& entry : m_active) {
        result.push_back({ entry.first, entry.second.remaining });
    }
    return result;
}

void BonusManager::restoreActive(const std::vector<ActiveBonusSnapshot>& snapshot)
{
    for (const ActiveBonusSnapshot& entry : snapshot) {
        activate(entry.type);
        auto it = m_active.find(entry.type);
        if (it != m_active.end()) {
            it->second.remaining = entry.remaining;
        }
    }
}

sf::Time BonusManager::durationFor(BonusType type)
{
    switch (type) {
        case BonusType::Fireball: return sf::seconds(7.f);
        case BonusType::FragileBlocks: return sf::seconds(6.f);
        case BonusType::WidePaddle: return sf::seconds(8.f);
        case BonusType::RemoveIndestructible: return sf::Time::Zero; // мгновенный, таймер не заводится.
    }
    return sf::seconds(5.f);
}

std::unique_ptr<IBonusCommand> BonusManager::createCommand(BonusType type)
{
    switch (type) {
        case BonusType::Fireball: return std::make_unique<FireballBonusCommand>(m_ball);
        case BonusType::FragileBlocks: return std::make_unique<FragileBlocksBonusCommand>(m_ball, m_gameState);
        case BonusType::WidePaddle: return std::make_unique<WidePaddleBonusCommand>(m_paddle);
        case BonusType::RemoveIndestructible: return std::make_unique<RemoveIndestructibleBonusCommand>(m_gameState);
    }

    assert(false && "BonusManager::createCommand: не обработан новый BonusType");
    return nullptr;
}
