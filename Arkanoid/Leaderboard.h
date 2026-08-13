// таблица рекордов
#pragma once
#include <string>
#include <vector>

struct HighscoreEntry {
    std::string name;
    int score = 0;
};

class Leaderboard {
public:
    // Читает записи из текстового файла в m_entries.
    void load(const std::string& filePath);

    // Записывает обратно в файл в том же формате.
    void save(const std::string& filePath);

    // Добавляет новую запись и пересортировывает m_entries по убыванию score.
    void addEntry(const std::string& name, int score);

    const std::vector<HighscoreEntry>& getEntries() const;

private:
    std::vector<HighscoreEntry> m_entries;

    void sortByScoreDescending();
};
