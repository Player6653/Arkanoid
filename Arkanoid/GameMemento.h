#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include "Brick.h"

// Паттерн "Хранитель" (Memento).
class GameMemento {
public:
    // Сохраняет снимок в текстовый файл. Возвращает false при ошибке записи.
    bool save(const std::string& filePath) const;

    // Загружает снимок из файла. Возвращает false, если файла нет или он повреждён.
    static bool load(const std::string& filePath, GameMemento& outMemento);

    int getLevel() const { return m_level; }
    int getDifficulty() const { return m_difficulty; }
    int getScore() const { return m_score; }

    // Сколько уровней нужно было пройти для победы В МОМЕНТ СОХРАНЕНИЯ — а не то, что сейчас стоит в Settings.
    int getLevelCount() const { return m_levelCount; }

private:
    friend class GameState;

    struct BrickSnapshot {
        BrickKind kind = BrickKind::Normal;
        float x = 0.f, y = 0.f, width = 0.f, height = 0.f;
        int hitsRemaining = 1;
    };

    int m_level = 1;
    int m_difficulty = 1;
    int m_score = 0;
    int m_levelCount = 5;
    sf::Vector2f m_ballPosition;
    sf::Vector2f m_ballVelocity;
    float m_paddleX = 0.f;
    std::vector<BrickSnapshot> m_bricks;
};
