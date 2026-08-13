#include "Paddle.h"
#include "GameState.h"

Paddle::Paddle(sf::Vector2f position, sf::Vector2f size)
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setFillColor(sf::Color::White);
}

void Paddle::update(sf::Time dt, const sf::RenderWindow& window)
{
    // Без фокуса окна не реагирует.
    if (!window.hasFocus()) {
        return;
    }

    bool keyboardUsed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_shape.move(-m_speed * dt.asSeconds(), 0.f);
        keyboardUsed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_shape.move(m_speed * dt.asSeconds(), 0.f);
        keyboardUsed = true;
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (!m_mouseInitialized) {
        m_lastMousePos = mousePos;
        m_mouseInitialized = true;
    }
    bool mouseMoved = (mousePos != m_lastMousePos);
    m_lastMousePos = mousePos;

    // Мышь ведёт платформу.
    if (!keyboardUsed && mouseMoved) {
        m_shape.setPosition(static_cast<float>(mousePos.x) - m_shape.getSize().x / 2.f, m_shape.getPosition().y);
    }

    clampToField();
}

void Paddle::clampToField()
{
    const float minX = 0.f;
    const float maxX = COLS * TILE_SIZE - m_shape.getSize().x;

    sf::Vector2f pos = m_shape.getPosition();
    if (pos.x < minX) {
        pos.x = minX;
    }
    else if (pos.x > maxX) {
        pos.x = maxX;
    }
    m_shape.setPosition(pos);
}

void Paddle::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::FloatRect Paddle::getBounds() const
{
    return m_shape.getGlobalBounds();
}
