#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameObject.h"
#include "IBallBehavior.h"

// Физика шарика.
class Ball : public GameObject {
public:
    Ball(sf::Vector2f position, sf::Vector2f velocity, float radius = 8.f);

    void update(sf::Time dt);

    // Отражает от границы поля шириной fieldWidth.
    void bounceOffWalls(float fieldWidth);

    // Отражает от платформы.
    void bounceOffPaddle();

    // Отражает от блока.
    void bounceOffBrick(const sf::FloatRect& brickBounds);

    // true, если шарик целиком ушёл ниже границы fieldHeight (мяч потерян).
    bool isBelow(float fieldHeight) const;

    sf::FloatRect getBounds() const override;
    void draw(sf::RenderWindow& window) const override;

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    void setPosition(sf::Vector2f position);

    // Задаёт скорость напрямую и считает её новой нормальной (сбрасывает множитель, накопленный через scaleVelocity).
    void setVelocity(sf::Vector2f velocity);

    // Домножает текущую скорость на factor (для бонуса огненный мяч).
    void scaleVelocity(float factor);

    // Скорость без учёта временных бонусов-разгонов — то, что нужно сохранять в файл (Хранитель), чтобы бонус не переживал save/load навсегда.
    sf::Vector2f getNormalizedVelocity() const;

    // Стратегия столкновения с блоками.
    const IBallBehavior& getBehavior() const;
    void setBehavior(std::unique_ptr<IBallBehavior> behavior);

    // Подсветка мяча для бонусов.
    void setTint(sf::Color color);
    void clearTint();

private:
    void updateColor();

    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    float m_speedMultiplier = 1.f;
    std::unique_ptr<IBallBehavior> m_behavior;
    sf::Color m_tint = sf::Color::White;
    bool m_hasTint = false;
};
