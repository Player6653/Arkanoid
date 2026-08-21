#pragma once
#include <SFML/Graphics.hpp>

// Общий интерфейс для всего, что рисуется на поле.
class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
};

// Общая проверка объект целиком улетел ниже границы поля".
inline bool isPositionBelowField(float centerY, float halfExtent, float fieldHeight)
{
    return centerY - halfExtent > fieldHeight;
}
