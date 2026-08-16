#include "Ball.h"

Ball::Ball(sf::Vector2f position, sf::Vector2f velocity, float radius)
    : m_velocity(velocity)
{
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius); // точка позиции — центр шарика, удобнее считать отскоки.
    m_shape.setPosition(position);
    m_shape.setFillColor(sf::Color::White);
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

void Ball::bounceOffBrick(const sf::FloatRect& brickBounds)
{
    sf::FloatRect ballBounds = getBounds();

    // Насколько шарик заехал в блок с каждой из четырёх сторон.
    float overlapLeft = ballBounds.left + ballBounds.width - brickBounds.left;
    float overlapRight = brickBounds.left + brickBounds.width - ballBounds.left;
    float overlapTop = ballBounds.top + ballBounds.height - brickBounds.top;
    float overlapBottom = brickBounds.top + brickBounds.height - ballBounds.top;

    float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
    float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

    sf::Vector2f pos = m_shape.getPosition();

    // Меньшее проникновение показывает, с какой стороны произошёл удар.
    if (minOverlapX < minOverlapY) {
        m_velocity.x = -m_velocity.x;
        pos.x += (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
    }
    else {
        m_velocity.y = -m_velocity.y;
        pos.y += (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;
    }

    m_shape.setPosition(pos);
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
