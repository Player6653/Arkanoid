#pragma once
#include <SFML/Graphics.hpp>

// Общий интерфейс для всего, что рисуется на поле.
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
};
