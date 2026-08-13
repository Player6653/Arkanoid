#include "Tetromino.h"

namespace {
    // Готовая таблица всех поворотов всех фигур.
    const int SHAPES[7][4][4][2] = {
        // I
        {
            { {0,1}, {1,1}, {2,1}, {3,1} },
            { {1,0}, {1,1}, {1,2}, {1,3} },
            { {0,2}, {1,2}, {2,2}, {3,2} },
            { {2,0}, {2,1}, {2,2}, {2,3} },
        },
        // O
        {
            { {1,1}, {2,1}, {1,2}, {2,2} },
            { {1,1}, {2,1}, {1,2}, {2,2} },
            { {1,1}, {2,1}, {1,2}, {2,2} },
            { {1,1}, {2,1}, {1,2}, {2,2} },
        },
        // T
        {
            { {1,0}, {0,1}, {1,1}, {2,1} },
            { {0,2}, {1,1}, {1,2}, {1,3} },
            { {1,2}, {2,2}, {3,2}, {2,3} },
            { {2,0}, {2,1}, {2,2}, {3,1} },
        },
        // S
        {
            { {1,0}, {2,0}, {0,1}, {1,1} },
            { {0,1}, {0,2}, {1,2}, {1,3} },
            { {1,3}, {2,3}, {2,2}, {3,2} },
            { {2,0}, {2,1}, {3,1}, {3,2} },
        },
        // Z
        {
            { {0,0}, {1,0}, {1,1}, {2,1} },
            { {0,2}, {0,3}, {1,1}, {1,2} },
            { {1,2}, {2,2}, {2,3}, {3,3} },
            { {2,1}, {2,2}, {3,0}, {3,1} },
        },
        // J
        {
            { {0,0}, {0,1}, {1,1}, {2,1} },
            { {0,3}, {1,1}, {1,2}, {1,3} },
            { {1,2}, {2,2}, {3,2}, {3,3} },
            { {2,0}, {2,1}, {2,2}, {3,0} },
        },
        // L
        {
            { {2,0}, {0,1}, {1,1}, {2,1} },
            { {0,1}, {1,1}, {1,2}, {1,3} },
            { {1,2}, {1,3}, {2,2}, {3,2} },
            { {2,0}, {2,1}, {2,2}, {3,2} },
        },
    };
}

const char* tetrominoTypeName(TetrominoType type)
{
    switch (type) {
        case TetrominoType::I: return "I";
        case TetrominoType::O: return "O";
        case TetrominoType::T: return "T";
        case TetrominoType::S: return "S";
        case TetrominoType::Z: return "Z";
        case TetrominoType::J: return "J";
        case TetrominoType::L: return "L";
    }
    return "?";
}

Tetromino::Tetromino(TetrominoType type)
    : m_type(type)
    , m_x(3)
    , m_y(0)
{
}

void Tetromino::moveBy(int dx, int dy)
{
    m_x += dx;
    m_y += dy;
}

void Tetromino::rotate()
{
    m_rotation = (m_rotation + 1) % 4;
}

std::array<sf::Vector2i, 4> Tetromino::getCells() const
{
    std::array<sf::Vector2i, 4> cells;
    int typeIndex = static_cast<int>(m_type);

    for (int i = 0; i < 4; ++i) {
        int offsetX = SHAPES[typeIndex][m_rotation][i][0];
        int offsetY = SHAPES[typeIndex][m_rotation][i][1];
        cells[i] = sf::Vector2i(m_x + offsetX, m_y + offsetY);
    }

    return cells;
}

std::array<sf::Vector2i, 4> Tetromino::previewCells(TetrominoType type)
{
    std::array<sf::Vector2i, 4> cells;
    int typeIndex = static_cast<int>(type);

    for (int i = 0; i < 4; ++i) {
        cells[i] = sf::Vector2i(SHAPES[typeIndex][0][i][0], SHAPES[typeIndex][0][i][1]);
    }

    return cells;
}
