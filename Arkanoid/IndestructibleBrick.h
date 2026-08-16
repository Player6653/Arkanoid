#pragma once
#include "Brick.h"

// Неразрушимый блок — мяч всегда от него отскакивает, разбить его нельзя.
class IndestructibleBrick : public Brick {
public:
    IndestructibleBrick(sf::Vector2f position, sf::Vector2f size);

    void onHit() override;
    bool countsTowardWin() const override;
};
