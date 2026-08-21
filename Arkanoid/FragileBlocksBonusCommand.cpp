#include "FragileBlocksBonusCommand.h"
#include "Ball.h"
#include "GameState.h"
#include "PowerUpColors.h"

FragileBlocksBonusCommand::FragileBlocksBonusCommand(Ball& ball, GameState& gameState)
    : m_ball(ball)
    , m_gameState(gameState)
{
}

void FragileBlocksBonusCommand::execute()
{
    m_gameState.setFragileBricksMode(true);
    m_ball.setTint(PowerUpColors::FragileBlocks);
}

void FragileBlocksBonusCommand::undo()
{
    m_gameState.setFragileBricksMode(false);
    m_ball.clearTint();
}
