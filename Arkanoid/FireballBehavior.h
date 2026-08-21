#pragma once
#include "IBallBehavior.h"

// Бонус огненный мяч.
class FireballBehavior : public IBallBehavior {
public:
    bool resolveBounce(bool brickWantsBounce, bool brickIsDestructible) const override;
    bool forcesInstantBreak() const override;
    sf::Color getColor() const override;
};
