#include "ResourceManager.h"
#include <cassert>

void ResourceManager::loadAll()
{
    bool fontLoaded = m_font.loadFromFile("Resources/Fonts/Roboto-Regular.ttf");
    assert(fontLoaded && "Не удалось загрузить шрифт Resources/Fonts/Roboto-Regular.ttf");

    bool rotateLoaded = m_rotateBuffer.loadFromFile("Resources/Sounds/rotate.wav");
    assert(rotateLoaded && "Не удалось загрузить Resources/Sounds/rotate.wav");

    bool lineClearLoaded = m_lineClearBuffer.loadFromFile("Resources/Sounds/lineclear.wav");
    assert(lineClearLoaded && "Не удалось загрузить Resources/Sounds/lineclear.wav");

    bool gameOverLoaded = m_gameOverBuffer.loadFromFile("Resources/Sounds/gameover.wav");
    assert(gameOverLoaded && "Не удалось загрузить Resources/Sounds/gameover.wav");

    bool swapLoaded = m_swapBuffer.loadFromFile("Resources/Sounds/swap.wav");
    assert(swapLoaded && "Не удалось загрузить Resources/Sounds/swap.wav");

    // стрим с диска
    bool menuMusicLoaded = m_menuMusic.openFromFile("Resources/Sounds/mainmenu.wav");
    assert(menuMusicLoaded && "Не удалось загрузить Resources/Sounds/mainmenu.wav");
    m_menuMusic.setLoop(true);

    bool gameplayMusicLoaded = m_gameplayMusic.openFromFile("Resources/Sounds/theme.wav");
    assert(gameplayMusicLoaded && "Не удалось загрузить Resources/Sounds/theme.wav");
    m_gameplayMusic.setLoop(true);
}

sf::Font& ResourceManager::getFont()
{
    return m_font;
}

sf::SoundBuffer& ResourceManager::getRotateBuffer()
{
    return m_rotateBuffer;
}

sf::SoundBuffer& ResourceManager::getLineClearBuffer()
{
    return m_lineClearBuffer;
}

sf::SoundBuffer& ResourceManager::getGameOverBuffer()
{
    return m_gameOverBuffer;
}

sf::SoundBuffer& ResourceManager::getSwapBuffer()
{
    return m_swapBuffer;
}

sf::Music& ResourceManager::getMenuMusic()
{
    return m_menuMusic;
}

sf::Music& ResourceManager::getGameplayMusic()
{
    return m_gameplayMusic;
}
