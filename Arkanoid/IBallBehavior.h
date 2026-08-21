#pragma once
#include <SFML/Graphics/Color.hpp>

// Стратегия поведения мяча при столкновении с блоком.
class IBallBehavior {
public:
    virtual ~IBallBehavior() = default;

    // brickWantsBounce — обычное решение блока (Brick::shouldBounceBall()).
    // brickIsDestructible — блок засчитывается в победу (не является неразрушимым).
    virtual bool resolveBounce(bool brickWantsBounce, bool brickIsDestructible) const
    {
        return brickWantsBounce;
    }

    // Ломает ли удар блок мгновенно, независимо от его прочности.
    virtual bool forcesInstantBreak() const
    {
        return false;
    }

    // Каким цветом красить мяч, пока действует эта стратегия.
    virtual sf::Color getColor() const
    {
        return sf::Color::White;
    }
};
