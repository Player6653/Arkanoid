#include "Brick.h"

Brick::Brick(sf::Vector2f position, sf::Vector2f size, sf::Color color)
{
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(color);
    m_shape.setOutlineThickness(1.f);
    m_shape.setOutlineColor(sf::Color(20, 20, 20));
}

void Brick::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Brick::getBounds() const
{
    return m_shape.getGlobalBounds();
}
