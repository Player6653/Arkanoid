#pragma once
#include "Brick.h"

// Крепкий блок.
class DurableBrick : public Brick {
public:
    DurableBrick(sf::Vector2f position, sf::Vector2f size, int hitsToDestroy);

    void onHit() override;

private:
    int m_hitsRemaining;

    // Чем меньше прочности осталось, тем темнее цвет.
    static sf::Color colorForHits(int hitsRemaining);
};
