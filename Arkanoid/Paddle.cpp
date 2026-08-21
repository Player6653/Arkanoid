#include "Paddle.h"
#include "GameState.h"

Paddle::Paddle(sf::Vector2f position, sf::Vector2f size)
    : m_baseSize(size)
    , m_state(std::make_unique<IPaddleState>())
{
    m_shape.setSize(size);
    m_shape.setPosition(position);
    m_shape.setFillColor(sf::Color(255, 140, 0)); // оранжевая.
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

void Paddle::resyncMouse(const sf::RenderWindow& window)
{
    m_lastMousePos = sf::Mouse::getPosition(window);
    m_mouseInitialized = true;
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

float Paddle::getX() const
{
    return m_shape.getPosition().x;
}

void Paddle::setX(float x)
{
    m_shape.setPosition(x, m_shape.getPosition().y);
    clampToField();
}

float Paddle::getNormalizedX() const
{
    float centerX = m_shape.getPosition().x + m_shape.getSize().x / 2.f;
    return centerX - m_baseSize.x / 2.f;
}

void Paddle::setState(std::unique_ptr<IPaddleState> state)
{
    m_state = std::move(state);

    float newWidth = m_baseSize.x * m_state->widthMultiplier();
    float oldWidth = m_shape.getSize().x;

    // Меняем ширину, оставляя центр платформы на месте.
    sf::Vector2f pos = m_shape.getPosition();
    pos.x -= (newWidth - oldWidth) / 2.f;

    m_shape.setSize(sf::Vector2f(newWidth, m_baseSize.y));
    m_shape.setPosition(pos);
    m_speed = m_baseSpeed * m_state->speedMultiplier();

    clampToField();
}
