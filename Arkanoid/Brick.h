#pragma once
#include "GameObject.h"

// Блок. Пока жив просто цветной прямоугольник на поле.
class Brick : public GameObject {
public:
    Brick(sf::Vector2f position, sf::Vector2f size, sf::Color color);

    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape m_shape;
};
