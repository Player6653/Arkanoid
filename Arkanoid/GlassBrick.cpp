#include "GlassBrick.h"
#include "BrickColors.h"

GlassBrick::GlassBrick(sf::Vector2f position, sf::Vector2f size)
    : Brick(position, size, BrickColors::Glass)
{
}

void GlassBrick::onHit()
{
    destroy();
}

bool GlassBrick::shouldBounceBall() const
{
    // Мяч пролетает сквозь стекло, не меняя направление.
    return false;
}

BrickKind GlassBrick::getKind() const
{
    return BrickKind::Glass;
}

int GlassBrick::getScoreValue() const
{
    return 5;
}
