#pragma once
#include <SFML/Graphics.hpp>

// Платформа игрока.
class Paddle {
public:
    Paddle(sf::Vector2f position, sf::Vector2f size);

    void update(sf::Time dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape m_shape;
    float m_speed = 350.f; // пикселей в секунду, постоянная скорость (без ускорения).

    // Чтобы отличать реальное движение мышт.
    sf::Vector2i m_lastMousePos;
    bool m_mouseInitialized = false;

    void clampToField();
};
