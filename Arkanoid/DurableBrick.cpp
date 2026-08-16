#include "DurableBrick.h"
#include "BrickColors.h"

DurableBrick::DurableBrick(sf::Vector2f position, sf::Vector2f size, int hitsToDestroy)
    : Brick(position, size, colorForHits(hitsToDestroy))
    , m_hitsRemaining(hitsToDestroy)
{
}

void DurableBrick::onHit()
{
    --m_hitsRemaining;

    if (m_hitsRemaining <= 0) {
        destroy();
    }
    else {
        setColor(colorForHits(m_hitsRemaining));
    }
}

sf::Color DurableBrick::colorForHits(int hitsRemaining)
{
    // 3 удара — светлая, 1 удар — тёмная.
    switch (hitsRemaining) {
        case 3: return BrickColors::DurableFresh;
        case 2: return BrickColors::DurableWorn;
        default: return BrickColors::DurableCritical;
    }
}
