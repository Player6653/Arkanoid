#pragma once
#include <SFML/Graphics.hpp>
#include <array>

enum class TetrominoType { I, O, T, S, Z, J, L };

const char* tetrominoTypeName(TetrominoType type);

class Tetromino {
public:
    explicit Tetromino(TetrominoType type);

    // Сдвигает точку привязки фигуры на (dx, dy).
    void moveBy(int dx, int dy);

    // Переключает фигуру на следующий из 4 готовых поворотов.
    void rotate();

    // Ставит поворот напрямую.
    void setRotation(int rotation) { m_rotation = rotation; }

    // 4 занятые клетки фигуры в АБСОЛЮТНЫХ координатах стакана
    std::array<sf::Vector2i, 4> getCells() const;

    // 4 клетки формы типа type при повороте 0.
    static std::array<sf::Vector2i, 4> previewCells(TetrominoType type);

    TetrominoType getType() const { return m_type; }
    int getRotation() const { return m_rotation; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }

private:
    TetrominoType m_type;
    int m_rotation = 0; // 0..3 магическое число xD
    int m_x;
    int m_y;
};