#include "FireballBehavior.h"
#include "PowerUpColors.h"

bool FireballBehavior::resolveBounce(bool brickWantsBounce, bool brickIsDestructible) const
{
    if (brickIsDestructible) {
        return false; // пробивает насквозь.
    }
    return brickWantsBounce; // от неразрушимого блока всё равно отскакивает.
}

bool FireballBehavior::forcesInstantBreak() const
{
    return true;
}

sf::Color FireballBehavior::getColor() const
{
    return PowerUpColors::Fireball;
}
