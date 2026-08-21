#pragma once
#include "IBonusCommand.h"

class Ball;

// Команда бонуса огненный мяч.
class FireballBonusCommand : public IBonusCommand {
public:
    explicit FireballBonusCommand(Ball& ball);

    void execute() override;
    void undo() override;

private:
    Ball& m_ball;
    static const float SPEED_MULTIPLIER;
};
