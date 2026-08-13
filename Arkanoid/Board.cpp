#include "Board.h"
#include "UiText.h"

Board::Board()
{
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            grid[y][x] = 0;
        }
    }
}

bool Board::isCollision(int x, int y) const
{
    if (x < 0 || x >= COLS || y >= ROWS) {
        return true;
    }
    if (y < 0) {
        // Клетка выше видимого поля
        return false;
    }
    return grid[y][x] != 0;
}

void Board::setFilled(int x, int y)
{
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        grid[y][x] = 1;
    }
}

bool Board::isRowFull(int y) const
{
    for (int x = 0; x < COLS; ++x) {
        if (grid[y][x] == 0) {
            return false;
        }
    }
    return true;
}

void Board::removeRow(int y)
{
    for (int r = y; r > 0; --r) {
        for (int x = 0; x < COLS; ++x) {
            grid[r][x] = grid[r - 1][x];
        }
    }
    for (int x = 0; x < COLS; ++x) {
        grid[0][x] = 0;
    }
}

int Board::clearFullLines()
{
    int linesCleared = 0;
    int y = ROWS - 1;

    while (y >= 0) {
        if (isRowFull(y)) {
            removeRow(y);
            ++linesCleared;
        }
        else {
            --y;
        }
    }

    return linesCleared;
}

void Board::draw(sf::RenderWindow& window)
{
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            if (grid[y][x] != 0) {
                drawTile(window, x, y, sf::Color::White);
            }
        }
    }
}
