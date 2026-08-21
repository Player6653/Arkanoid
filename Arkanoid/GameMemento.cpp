#include "GameMemento.h"
#include <fstream>

namespace {
    // Разумный потолок — реальная доска не больше пары сотен блоков. 
    const std::size_t MAX_REASONABLE_BRICK_COUNT = 10000;
}

bool GameMemento::save(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    file << m_level << " " << m_difficulty << " " << m_score << " " << m_levelCount << "\n";
    file << m_ballPosition.x << " " << m_ballPosition.y << " "
        << m_ballVelocity.x << " " << m_ballVelocity.y << "\n";
    file << m_paddleX << "\n";
    file << m_bricks.size() << "\n";

    for (const BrickSnapshot& brick : m_bricks) {
        file << static_cast<int>(brick.kind) << " "
            << brick.x << " " << brick.y << " " << brick.width << " " << brick.height << " "
            << brick.hitsRemaining << "\n";
    }

    return true;
}

bool GameMemento::load(const std::string& filePath, GameMemento& outMemento)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    GameMemento memento;

    if (!(file >> memento.m_level >> memento.m_difficulty >> memento.m_score >> memento.m_levelCount)) {
        return false;
    }
    if (!(file >> memento.m_ballPosition.x >> memento.m_ballPosition.y
        >> memento.m_ballVelocity.x >> memento.m_ballVelocity.y)) {
        return false;
    }
    if (!(file >> memento.m_paddleX)) {
        return false;
    }

    std::size_t brickCount = 0;
    if (!(file >> brickCount) || brickCount > MAX_REASONABLE_BRICK_COUNT) {
        return false;
    }

    memento.m_bricks.reserve(brickCount);
    for (std::size_t i = 0; i < brickCount; ++i) {
        int kind = 0;
        BrickSnapshot snapshot;
        if (!(file >> kind >> snapshot.x >> snapshot.y >> snapshot.width >> snapshot.height >> snapshot.hitsRemaining)) {
            return false;
        }
        snapshot.kind = static_cast<BrickKind>(kind);
        memento.m_bricks.push_back(snapshot);
    }

    outMemento = std::move(memento);
    return true;
}
