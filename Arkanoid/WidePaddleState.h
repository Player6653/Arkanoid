#pragma once
#include "IPaddleState.h"

// Бонус платформы.
class WidePaddleState : public IPaddleState {
public:
    float widthMultiplier() const override;
    float speedMultiplier() const override;
};
