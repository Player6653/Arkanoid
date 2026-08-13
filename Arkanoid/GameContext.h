// набор общих сервисов
#pragma once
#include "Settings.h"

class ResourceManager;
class Leaderboard;

struct GameContext {
    ResourceManager& resources;
    Settings& settings;
    Leaderboard& leaderboard;
};
