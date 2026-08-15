// общие пользовательские настройки.
#pragma once

class Settings {
public:
    bool isSoundOn() const { return m_soundOn; }
    bool isMusicOn() const { return m_musicOn; }
    int getDifficulty() const { return m_difficulty; }
    int getBrickRows() const { return m_brickRows; }

    void toggleSound() { m_soundOn = !m_soundOn; }
    void toggleMusic() { m_musicOn = !m_musicOn; }

    // Значения должны быть зациклены.
    void increaseDifficulty()
    {
        m_difficulty = (m_difficulty < 3) ? m_difficulty + 1 : 1;
    }

    void decreaseDifficulty()
    {
        m_difficulty = (m_difficulty > 1) ? m_difficulty - 1 : 3;
    }

    void increaseBrickRows()
    {
        m_brickRows = (m_brickRows < 10) ? m_brickRows + 1 : 1;
    }

    void decreaseBrickRows()
    {
        m_brickRows = (m_brickRows > 1) ? m_brickRows - 1 : 10;
    }

private:
    bool m_soundOn = true;
    bool m_musicOn = true;

    // 1 - легко, 2 - средне, 3 - сложно.
    int m_difficulty = 1;

    // 1 и до 10 рядов блоков на игровом поле.
    int m_brickRows = 4;
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

// Множитель очков.
inline float difficultyScoreMultiplier(int difficulty)
{
    switch (difficulty) {
        case 1: return 1.f;
        case 2: return 1.25f;
        case 3: return 1.5f;
        default: return 1.f;
    }
}
