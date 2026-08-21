#pragma once
#include "Brick.h"
#include <memory>
#include <random>

// Порождающий паттерн "Фабричный метод".
class BrickFactory {
public:
    explicit BrickFactory(int level);
    virtual ~BrickFactory() = default;

    // Обычная точка входа.
    std::unique_ptr<Brick> createRandomBrick(sf::Vector2f position, sf::Vector2f size, std::mt19937& rng) const;

    // Точный блок нужного типа — нужен, чтобы Хранитель (Memento) мог восстановить сохранённую раскладку без повторного бросания кубика.
    std::unique_ptr<Brick> createExactBrick(BrickKind kind, sf::Vector2f position, sf::Vector2f size, int durableHitsRemaining = 3) const;

protected:
    // Собственно фабричный метод — наследники могут переопределить его, чтобы подставлять свои классы блоков вместо стандартных.
    virtual std::unique_ptr<Brick> createBrick(BrickKind kind, sf::Vector2f position, sf::Vector2f size, int durableHitsRemaining) const;

private:
    int m_level;
    float m_indestructibleChance;
    float m_glassChance;
    float m_durableChance;
};
