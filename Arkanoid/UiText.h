#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"

inline void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& text,
    float x, float y, unsigned int size = 20, sf::Color color = sf::Color::White)
{
    sf::String utf8Text = sf::String::fromUtf8(text.begin(), text.end());
    sf::Text label(utf8Text, font, size);
    label.setPosition(x, y);
    label.setFillColor(color);
    window.draw(label);
}

inline void drawTile(sf::RenderWindow& window, int x, int y, sf::Color color,
    sf::Vector2f origin = sf::Vector2f(0.f, 0.f))
{
    sf::RectangleShape rect(sf::Vector2f(TILE_SIZE - 1.f, TILE_SIZE - 1.f));
    rect.setPosition(origin.x + x * TILE_SIZE, origin.y + y * TILE_SIZE);
    rect.setFillColor(color);
    window.draw(rect);
}
