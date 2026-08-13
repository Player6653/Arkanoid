#pragma once
#include <SFML/Audio.hpp>
#include "Settings.h"

inline void playSfx(const Settings& settings, sf::Sound& sound)
{
    if (settings.soundOn) {
        sound.play();
    }
}

// Останавливает один трек и запускает другой
inline void switchMusic(const Settings& settings, sf::Music& stop, sf::Music& start)
{
    stop.stop();
    if (settings.musicOn && start.getStatus() != sf::Music::Playing) {
        start.play();
    }
}

// play(), вызванный на уже играющем треке.
inline void playIfNotPlaying(sf::Music& music)
{
    if (music.getStatus() != sf::Music::Playing) {
        music.play();
    }
}
