#include "FireballBonusCommand.h"
#include "Ball.h"
#include "FireballBehavior.h"
#include <memory>

const float FireballBonusCommand::SPEED_MULTIPLIER = 1.5f;

FireballBonusCommand::FireballBonusCommand(Ball& ball)
    : m_ball(ball)
{
}

void FireballBonusCommand::execute()
{
    m_ball.setBehavior(std::make_unique<FireballBehavior>());
    m_ball.scaleVelocity(SPEED_MULTIPLIER);
}

void FireballBonusCommand::undo()
{
    m_ball.scaleVelocity(1.f / SPEED_MULTIPLIER);
    m_ball.setBehavior(std::make_unique<IBallBehavior>());
}
