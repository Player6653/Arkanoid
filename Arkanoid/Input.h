#pragma once
#include <SFML/Graphics.hpp>

inline bool isBackKey(const sf::Event& event)
{
    return event.type == sf::Event::KeyPressed
        && (event.key.code == sf::Keyboard::Escape
            || event.key.code == sf::Keyboard::Enter
            || event.key.code == sf::Keyboard::Space);
}

// Не даёт экрану среагироватб, которые ещё физически зажаты в момент.
class StaleKeyGuard {
public:
    StaleKeyGuard()
        : m_staleEnter(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        , m_staleSpace(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
    }

    bool accept(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Enter) {
                m_staleEnter = false;
            }
            if (event.key.code == sf::Keyboard::Space) {
                m_staleSpace = false;
            }
            return true;
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter && m_staleEnter) {
                return false;
            }
            if (event.key.code == sf::Keyboard::Space && m_staleSpace) {
                return false;
            }
        }
        return true;
    }

private:
    bool m_staleEnter;
    bool m_staleSpace;
};
