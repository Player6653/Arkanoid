#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

#include "Board.h"
#include "GameContext.h"
#include "ResourceManager.h"
#include "Leaderboard.h"
#include "Settings.h"
#include "StateManager.h"
#include "States/MenuState.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLS * TILE_SIZE + UI_PANEL_WIDTH, ROWS * TILE_SIZE), "Tetris");

    ResourceManager resources;
    resources.loadAll();

    Settings settings;
    Leaderboard leaderboard;
    leaderboard.load("highscores.txt");

    GameContext context{ resources, settings, leaderboard };

    StateManager stateManager(std::make_unique<MenuState>(context));

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            stateManager.handleInput(event);
        }

        sf::Time dt = clock.restart();
        stateManager.update(dt);

        if (stateManager.wantsExit()) {
            window.close();
        }

        window.clear();
        stateManager.draw(window);
        window.display();
    }

    leaderboard.save("highscores.txt");

    return 0;
}
