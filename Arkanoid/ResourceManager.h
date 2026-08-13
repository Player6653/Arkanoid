// ResourceManager.h — класс, который грузит и хранит все текстуры/шрифты/звуки игры
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager {
public:
    // Грузит шрифт, звуковые эффекты и музыку (см. ResourceManager.cpp).
    void loadAll();

    sf::Font& getFont();

    sf::SoundBuffer& getRotateBuffer();
    sf::SoundBuffer& getLineClearBuffer();
    sf::SoundBuffer& getGameOverBuffer();
    sf::SoundBuffer& getSwapBuffer();
    sf::Music& getMenuMusic();
    sf::Music& getGameplayMusic();

private:
    sf::Font m_font;

    sf::SoundBuffer m_rotateBuffer;
    sf::SoundBuffer m_lineClearBuffer;
    sf::SoundBuffer m_gameOverBuffer;
    sf::SoundBuffer m_swapBuffer;

    sf::Music m_menuMusic;
    sf::Music m_gameplayMusic;
};
