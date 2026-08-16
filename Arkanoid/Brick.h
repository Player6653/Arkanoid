#pragma once
#include "GameObject.h"

// Блок. Базовое поведение — разбивается с одного удара.
class Brick : public GameObject {
public:
    Brick(sf::Vector2f position, sf::Vector2f size, sf::Color color);

    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    // Вызывается GameState при столкновении с мячом.
    virtual void onHit();

    bool isDestroyed() const;

    // true, если от этого блока мяч должен отскакивать (у GlassBrick — false).
    virtual bool shouldBounceBall() const;

    // true, если блок нужно разбить для победы.
    virtual bool countsTowardWin() const;

    virtual ~Brick() = default;

protected:
    // Помечает блок разрушенным.
    void destroy();

    // Позволяет наследникам перекрашивать блок.
    void setColor(sf::Color color);

private:
    sf::RectangleShape m_shape;
    bool m_destroyed = false;
};
