#include "IndestructibleBrick.h"
#include "BrickColors.h"

IndestructibleBrick::IndestructibleBrick(sf::Vector2f position, sf::Vector2f size)
    : Brick(position, size, BrickColors::Indestructible)
{
}

void IndestructibleBrick::onHit()
{

}

bool IndestructibleBrick::countsTowardWin() const
{
    return false;
}
