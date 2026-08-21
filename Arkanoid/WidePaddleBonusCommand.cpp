#include "WidePaddleBonusCommand.h"
#include "Paddle.h"
#include "WidePaddleState.h"
#include "IPaddleState.h"
#include <memory>

WidePaddleBonusCommand::WidePaddleBonusCommand(Paddle& paddle)
    : m_paddle(paddle)
{
}

void WidePaddleBonusCommand::execute()
{
    m_paddle.setState(std::make_unique<WidePaddleState>());
}

void WidePaddleBonusCommand::undo()
{
    m_paddle.setState(std::make_unique<IPaddleState>());
}
