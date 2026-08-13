// общие пользовательские настройки 
#pragma once

struct Settings {
    bool soundOn = true;
    bool musicOn = true;

    // 1 - легко, 2 - средне, 3 - сложно
    int difficulty = 1;
};

inline const char* difficultyName(int difficulty)
{
    switch (difficulty) {
        case 1: return "Лёгкий";
        case 2: return "Средний";
        case 3: return "Сложный";
        default: return "хз";
    }
}
