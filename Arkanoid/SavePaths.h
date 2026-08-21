#pragma once
#include <cstdio>

// Единое место, куда игра сохраняется/из которого загружается.
inline const char* savePath()
{
    return "savegame.txt";
}

// Забег закончился (победа или поражение) — продолжать больше нечего.
inline void deleteSaveFile()
{
    std::remove(savePath());
}
