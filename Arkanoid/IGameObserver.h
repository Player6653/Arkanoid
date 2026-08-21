#pragma once

// Наблюдатель за событиями GameState (Subject).
class IGameObserver {
public:
    virtual ~IGameObserver() = default;

    virtual void onBrickDestroyed(int scoreValue) {}
    virtual void onLevelComplete() {}
};
