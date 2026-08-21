#pragma once
#include "GameObject.h"

// Тип блока — нужен фабрике и Хранителю (Memento).
enum class BrickKind { Normal, Durable, Glass, Indestructible };

// Блок. Базовое поведение — разбивается с одного удара.
class Brick : public GameObject {
public:
    Brick(sf::Vector2f position, sf::Vector2f size, sf::Color color);

    void draw(sf::RenderWindow& window) const override;
    sf::FloatRect getBounds() const override;

    // Вызывается GameState при столкновении с мячом.
    virtual void onHit();

    // Разбивает блок сразу, игнорируя прочность (для бонуса хрупкие блоки).
    virtual void forceDestroy();

    bool isDestroyed() const;

    // true, если от этого блока мяч должен отскакивать (у GlassBrick — false).
    virtual bool shouldBounceBall() const;

    // true, если блок нужно разбить для победы.
    virtual bool countsTowardWin() const;

    // Тип блока — для BrickFactory (сериализация в Memento) и легенды в UI.
    virtual BrickKind getKind() const;

    // Сколько очков даёт разрушение этого блока.
    virtual int getScoreValue() const;

    // Сколько ударов осталось выдержать (актуально только для DurableBrick).
    virtual int getHitsRemaining() const;

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
