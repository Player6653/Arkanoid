#pragma once
#include "GameObject.h"
#include "BonusType.h"

// Падающий бонус: появляется на месте разрушенного блока и летит вниз.
class PowerUp : public GameObject {
public:
    PowerUp(sf::Vector2f position, BonusType type);

    void update(sf::Time dt);

    // true, если бонус целиком улетел ниже границы fieldHeight (пропущен).
    bool isBelow(float fieldHeight) const;

    BonusType getType() const;

    sf::FloatRect getBounds() const override;
    void draw(sf::RenderWindow& window) const override;

private:
    sf::RectangleShape m_shape;
    BonusType m_type;
    float m_fallSpeed = 120.f;
};
