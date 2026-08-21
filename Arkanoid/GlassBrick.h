#pragma once
#include "Brick.h"

// Стеклянный блок — разбивается с одного касания и не отражает мяч (тот пролетает сквозь).
class GlassBrick : public Brick {
public:
    GlassBrick(sf::Vector2f position, sf::Vector2f size);

    void onHit() override;
    bool shouldBounceBall() const override;

    BrickKind getKind() const override;
    int getScoreValue() const override;
};
