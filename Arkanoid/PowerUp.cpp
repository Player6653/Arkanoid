#include "PowerUp.h"
#include "PowerUpColors.h"

namespace {
    sf::Color colorForType(BonusType type)
    {
        switch (type) {
            case BonusType::Fireball: return PowerUpColors::Fireball;
            case BonusType::FragileBlocks: return PowerUpColors::FragileBlocks;
            case BonusType::WidePaddle: return PowerUpColors::WidePaddle;
            case BonusType::RemoveIndestructible: return PowerUpColors::RemoveIndestructible;
        }
        return sf::Color::White;
    }
}

PowerUp::PowerUp(sf::Vector2f position, BonusType type)
    : m_type(type)
{
    const float size = 16.f;
    m_shape.setSize(sf::Vector2f(size, size));
    m_shape.setOrigin(size / 2.f, size / 2.f);
    m_shape.setPosition(position);
    m_shape.setFillColor(colorForType(type));
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color(20, 20, 20));
}

void PowerUp::update(sf::Time dt)
{
    m_shape.move(0.f, m_fallSpeed * dt.asSeconds());
}

bool PowerUp::isBelow(float fieldHeight) const
{
    return isPositionBelowField(m_shape.getPosition().y, m_shape.getSize().y / 2.f, fieldHeight);
}

BonusType PowerUp::getType() const
{
    return m_type;
}

sf::FloatRect PowerUp::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void PowerUp::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}
