// общие пользовательские настройки
#pragma once

class Settings {
public:
    bool isSoundOn() const { return m_soundOn; }
    bool isMusicOn() const { return m_musicOn; }
    int getDifficulty() const { return m_difficulty; }

    void toggleSound() { m_soundOn = !m_soundOn; }
    void toggleMusic() { m_musicOn = !m_musicOn; }

    // Возвращают true, только если значение реально изменилось.
    bool increaseDifficulty()
    {
        if (m_difficulty < 3) {
            ++m_difficulty;
            return true;
        }
        return false;
    }

    bool decreaseDifficulty()
    {
        if (m_difficulty > 1) {
            --m_difficulty;
            return true;
        }
        return false;
    }

private:
    bool m_soundOn = true;
    bool m_musicOn = true;

    // 1 - легко, 2 - средне, 3 - сложно
    int m_difficulty = 1;
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
