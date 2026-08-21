#include "RemoveIndestructibleBonusCommand.h"
#include "GameState.h"

RemoveIndestructibleBonusCommand::RemoveIndestructibleBonusCommand(GameState& gameState)
    : m_gameState(gameState)
{
}

void RemoveIndestructibleBonusCommand::execute()
{
    m_gameState.removeRandomIndestructibleBrick();
}

void RemoveIndestructibleBonusCommand::undo()
{
    // Убранный блок не восстанавливается — эффект насовсем.
}

bool RemoveIndestructibleBonusCommand::isInstant() const
{
    return true;
}
