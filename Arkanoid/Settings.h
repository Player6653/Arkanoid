// общие пользовательские настройки.
#pragma once

class Settings {
public:
    bool isSoundOn() const { return m_soundOn; }
    bool isMusicOn() const { return m_musicOn; }
    int getDifficulty() const { return m_difficulty; }
    int getBrickRows() const { return m_brickRows; }
    int getLevelCount() const { return m_levelCount; }

    // 0 означает "без ограничения" так же трактует и sf::RenderWindow::setFramerateLimit.
    int getFpsLimit() const { return FPS_LIMIT_OPTIONS[m_fpsLimitIndex]; }

    void toggleSound() { m_soundOn = !m_soundOn; }
    void toggleMusic() { m_musicOn = !m_musicOn; }

    // Значения зациклены.
    void increaseFpsLimit()
    {
        m_fpsLimitIndex = (m_fpsLimitIndex + 1) % FPS_LIMIT_OPTION_COUNT;
    }

    void decreaseFpsLimit()
    {
        m_fpsLimitIndex = (m_fpsLimitIndex + FPS_LIMIT_OPTION_COUNT - 1) % FPS_LIMIT_OPTION_COUNT;
    }

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

    void increaseLevelCount()
    {
        m_levelCount = (m_levelCount < 10) ? m_levelCount + 1 : 1;
    }

    void decreaseLevelCount()
    {
        m_levelCount = (m_levelCount > 1) ? m_levelCount - 1 : 10;
    }

private:
    bool m_soundOn = true;
    bool m_musicOn = true;

    // 1 - легко, 2 - средне, 3 - сложно.
    int m_difficulty = 2;

    // 1 и до 10 рядов блоков на игровом поле (на первом уровне).
    int m_brickRows = 5;

    // Сколько уровней нужно пройти для победы.
    int m_levelCount = 3;

    // Варианты лимита кадров в секунду; 0 - без ограничения.
    static constexpr int FPS_LIMIT_OPTIONS[] = { 30, 60, 90, 120, 144, 240, 280, 0 };
    static constexpr int FPS_LIMIT_OPTION_COUNT = 8;
    int m_fpsLimitIndex = 3; // по умолчанию 120.
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
