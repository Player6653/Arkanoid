#pragma once
#include "IBonusCommand.h"

class GameState;
class Ball;

// Команда бонуса хрупкие блоки.
class FragileBlocksBonusCommand : public IBonusCommand {
public:
    FragileBlocksBonusCommand(Ball& ball, GameState& gameState);

    void execute() override;
    void undo() override;

private:
    Ball& m_ball;
    GameState& m_gameState;
};
