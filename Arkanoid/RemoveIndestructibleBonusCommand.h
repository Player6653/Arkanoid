#pragma once
#include "IBonusCommand.h"

class GameState;

// Команда бонуса "Динамит": мгновенно убирает с поля один случайный неразрушимый блок.
class RemoveIndestructibleBonusCommand : public IBonusCommand {
public:
    explicit RemoveIndestructibleBonusCommand(GameState& gameState);

    void execute() override;
    void undo() override;
    bool isInstant() const override;

private:
    GameState& m_gameState;
};
