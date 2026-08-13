// набор общих сервисов.
#pragma once
#include "Settings.h"

class ResourceManager;
class Leaderboard;

class GameContext {
public:
    GameContext(ResourceManager& resources, Settings& settings, Leaderboard& leaderboard)
        : m_resources(resources)
        , m_settings(settings)
        , m_leaderboard(leaderboard)
    {
    }

    ResourceManager& getResources() { return m_resources; }
    Settings& getSettings() { return m_settings; }
    Leaderboard& getLeaderboard() { return m_leaderboard; }

private:
    ResourceManager& m_resources;
    Settings& m_settings;
    Leaderboard& m_leaderboard;
};
