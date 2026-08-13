#pragma once
#include <SFML/Graphics.hpp>

const short ROWS = 20;
const short COLS = 10;
const short TILE_SIZE = 30; // Размер одной клеточки в пикселях для отрисовки
const short UI_PANEL_WIDTH = 200; // Место справа от стакана под счёт/уровень/текст

class Board {
public:
    Board();

    // Главная функция проверок.
    bool isCollision(int x, int y) const;

    // Записывает клетку упавшей фигуры в сетку.
    void setFilled(int x, int y);

    // Проверяет и удаляет все заполненные строки.
    int clearFullLines();

    void draw(sf::RenderWindow& window);

private:
    // массив стакана.
    int grid[ROWS][COLS];

    // Заполнена ли строка целиком.
    bool isRowFull(int y) const;

    // Удаляет строку y
    void removeRow(int y);
};
