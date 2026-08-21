#include "BrickFactory.h"
#include "DurableBrick.h"
#include "GlassBrick.h"
#include "IndestructibleBrick.h"
#include "BrickColors.h"
#include <algorithm>

namespace {
    // Базовые доли клеток под каждый спец-тип (суммарно должны быть <= 1.0).
    const float BASE_INDESTRUCTIBLE_CHANCE = 0.05f;
    const float BASE_GLASS_CHANCE = 0.08f;
    const float BASE_DURABLE_CHANCE = 0.15f;

    // На каждый уровень свыше первого блоки чуть крепче, но не бесконечно. Неразрушимые блоки не считаются в победу.
    const float PER_LEVEL_INDESTRUCTIBLE_HARDNESS = 0.01f;
    const float PER_LEVEL_DURABLE_HARDNESS = 0.02f;
    const int MAX_HARDNESS_LEVELS = 4;
}

BrickFactory::BrickFactory(int level)
    : m_level(level)
{
    int cappedLevels = std::min(level - 1, MAX_HARDNESS_LEVELS);
    m_indestructibleChance = BASE_INDESTRUCTIBLE_CHANCE + cappedLevels * PER_LEVEL_INDESTRUCTIBLE_HARDNESS;
    m_glassChance = BASE_GLASS_CHANCE;
    m_durableChance = BASE_DURABLE_CHANCE + cappedLevels * PER_LEVEL_DURABLE_HARDNESS;
}

std::unique_ptr<Brick> BrickFactory::createRandomBrick(sf::Vector2f position, sf::Vector2f size, std::mt19937& rng) const
{
    std::uniform_real_distribution<float> roll(0.f, 1.f);
    float r = roll(rng);

    if (r < m_indestructibleChance) {
        return createBrick(BrickKind::Indestructible, position, size, 3);
    }
    if (r < m_indestructibleChance + m_glassChance) {
        return createBrick(BrickKind::Glass, position, size, 3);
    }
    if (r < m_indestructibleChance + m_glassChance + m_durableChance) {
        return createBrick(BrickKind::Durable, position, size, 3);
    }
    return createBrick(BrickKind::Normal, position, size, 3);
}

std::unique_ptr<Brick> BrickFactory::createExactBrick(BrickKind kind, sf::Vector2f position, sf::Vector2f size, int durableHitsRemaining) const
{
    return createBrick(kind, position, size, durableHitsRemaining);
}

std::unique_ptr<Brick> BrickFactory::createBrick(BrickKind kind, sf::Vector2f position, sf::Vector2f size, int durableHitsRemaining) const
{
    switch (kind) {
        case BrickKind::Indestructible:
            return std::make_unique<IndestructibleBrick>(position, size);
        case BrickKind::Glass:
            return std::make_unique<GlassBrick>(position, size);
        case BrickKind::Durable:
            return std::make_unique<DurableBrick>(position, size, durableHitsRemaining);
        case BrickKind::Normal:
        default:
            return std::make_unique<Brick>(position, size, BrickColors::Normal);
    }
}
