// таблица рекордов 
#include "Leaderboard.h"
#include <fstream>
#include <algorithm>

void Leaderboard::load(const std::string& filePath)
{
    m_entries.clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        // Файла ещё нет (первый запуск).
        return;
    }

    std::string name;
    int score;
    while (file >> name >> score) {
        m_entries.push_back({ name, score });
    }

    sortByScoreDescending();
}

void Leaderboard::save(const std::string& filePath)
{
    std::ofstream file(filePath);
    for (const HighscoreEntry& entry : m_entries) {
        file << entry.name << " " << entry.score << "\n";
    }
}

void Leaderboard::addEntry(const std::string& name, int score)
{
    m_entries.push_back({ name, score });
    sortByScoreDescending();
}

void Leaderboard::sortByScoreDescending()
{
    std::sort(m_entries.begin(), m_entries.end(),
        [](const HighscoreEntry& a, const HighscoreEntry& b) {
            return a.score > b.score;
        });
}

const std::vector<HighscoreEntry>& Leaderboard::getEntries() const
{
    return m_entries;
}
