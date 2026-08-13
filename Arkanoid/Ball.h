#pragma once
#include <SFML/Graphics.hpp>

// Физика шарика.
class Ball {
public:
    Ball(sf::Vector2f position, sf::Vector2f velocity, float radius = 8.f);

    void update(sf::Time dt);

    // Отражает от границы поля шириной fieldWidth.
    void bounceOffWalls(float fieldWidth);

    // Отражает от платформы.
    void bounceOffPaddle();

    // true, если шарик целиком ушёл ниже границы fieldHeight (мяч потерян).
    bool isBelow(float fieldHeight) const;

    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow& window) const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
};
