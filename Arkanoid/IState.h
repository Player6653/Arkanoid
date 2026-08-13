#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class IState {
public:
    virtual ~IState() = default;

    virtual void handleInput(const sf::Event& event) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    virtual void update(sf::Time dt) {}

    virtual std::unique_ptr<IState> nextState() { return nullptr; }

    virtual bool wantsExit() const { return false; }
};
