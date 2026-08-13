#include "Ball.h"

Ball::Ball(sf::Vector2f position, sf::Vector2f velocity, float radius)
    : m_velocity(velocity)
{
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius); // точка позиции — центр шарика, удобнее считать отскоки.
    m_shape.setPosition(position);
    m_shape.setFillColor(sf::Color::Yellow);
}

void Ball::update(sf::Time dt)
{
    m_shape.move(m_velocity * dt.asSeconds());
}

void Ball::bounceOffWalls(float fieldWidth)
{
    sf::Vector2f pos = m_shape.getPosition();
    const float radius = m_shape.getRadius();

    if (pos.x - radius < 0.f) {
        pos.x = radius;
        m_velocity.x = -m_velocity.x;
    }
    else if (pos.x + radius > fieldWidth) {
        pos.x = fieldWidth - radius;
        m_velocity.x = -m_velocity.x;
    }

    if (pos.y - radius < 0.f) {
        pos.y = radius;
        m_velocity.y = -m_velocity.y;
    }

    m_shape.setPosition(pos);
}

void Ball::bounceOffPaddle()
{
    // Отражаем, только если шарик летел вниз — иначе повторный вызов.
    if (m_velocity.y > 0.f) {
        m_velocity.y = -m_velocity.y;
    }
}

bool Ball::isBelow(float fieldHeight) const
{
    return m_shape.getPosition().y - m_shape.getRadius() > fieldHeight;
}

sf::FloatRect Ball::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Ball::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}
