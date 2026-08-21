#include "Ball.h"

Ball::Ball(sf::Vector2f position, sf::Vector2f velocity, float radius)
    : m_velocity(velocity)
    , m_behavior(std::make_unique<IBallBehavior>())
{
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius); // точка позиции - центр шарика, удобнее считать отскоки.
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
    return isPositionBelowField(m_shape.getPosition().y, m_shape.getRadius(), fieldHeight);
}

sf::FloatRect Ball::getBounds() const
{
    return m_shape.getGlobalBounds();
}

void Ball::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::Vector2f Ball::getPosition() const
{
    return m_shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const
{
    return m_velocity;
}

void Ball::setPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);
}

void Ball::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
    m_speedMultiplier = 1.f; // это и есть новая нормальная скорость.
}

void Ball::scaleVelocity(float factor)
{
    m_velocity *= factor;
    m_speedMultiplier *= factor;
}

sf::Vector2f Ball::getNormalizedVelocity() const
{
    return m_velocity / m_speedMultiplier;
}

const IBallBehavior& Ball::getBehavior() const
{
    return *m_behavior;
}

void Ball::setBehavior(std::unique_ptr<IBallBehavior> behavior)
{
    m_behavior = std::move(behavior);
    updateColor();
}

void Ball::setTint(sf::Color color)
{
    m_tint = color;
    m_hasTint = true;
    updateColor();
}

void Ball::clearTint()
{
    m_hasTint = false;
    updateColor();
}

void Ball::updateColor()
{
    sf::Color behaviorColor = m_behavior->getColor();
    if (behaviorColor != sf::Color::White) {
        m_shape.setFillColor(behaviorColor); // особая стратегия важнее подсветки.
    }
    else if (m_hasTint) {
        m_shape.setFillColor(m_tint);
    }
    else {
        m_shape.setFillColor(sf::Color::White);
    }
}
