#pragma once

// Состояние платформы.
class IPaddleState {
public:
    virtual ~IPaddleState() = default;

    virtual float widthMultiplier() const
    {
        return 1.f;
    }

    virtual float speedMultiplier() const
    {
        return 1.f;
    }
};
