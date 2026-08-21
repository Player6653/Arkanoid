#pragma once
#include "IBonusCommand.h"

class Paddle;

// Команда бонуса платформы.
class WidePaddleBonusCommand : public IBonusCommand {
public:
    explicit WidePaddleBonusCommand(Paddle& paddle);

    void execute() override;
    void undo() override;

private:
    Paddle& m_paddle;
};
