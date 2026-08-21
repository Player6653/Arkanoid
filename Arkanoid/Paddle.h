#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameObject.h"
#include "IPaddleState.h"

// Платформа игрока.
class Paddle : public GameObject {
public:
    Paddle(sf::Vector2f position, sf::Vector2f size);

    void update(sf::Time dt, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const override;

    // Забывает прошлую позицию мыши, считая текущую точкой отсчёта.
    void resyncMouse(const sf::RenderWindow& window);

    sf::FloatRect getBounds() const override;

    float getX() const;
    void setX(float x);

    // Левый край платформы так, как будто она сейчас обычной ширины (центр сохраняется).
    float getNormalizedX() const;

    // Состояние платформы — см. IPaddleState/WidePaddleState. Меняет ширину/скорость, сохраняя центр платформы на месте.
    void setState(std::unique_ptr<IPaddleState> state);

private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_baseSize;
    float m_baseSpeed = 350.f; // пикселей в секунду, постоянная скорость (без ускорения).
    float m_speed = m_baseSpeed; // фактическая скорость с учётом состояния платформы (см. setState()).
    std::unique_ptr<IPaddleState> m_state;

    // Чтобы отличать реальное движение мышь.
    sf::Vector2i m_lastMousePos;
    bool m_mouseInitialized = false;

    void clampToField();
};
