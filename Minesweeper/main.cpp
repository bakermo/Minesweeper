#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "Board.h" // remove some of these that arent getting used
                    // todo: include leaker?

using namespace std;
using namespace sf;

Board LoadBoardFromConfig()
{
    ifstream file("boards/config.cfg");
    int config[3] = {0, 0, 0};
    if (file.is_open())
    {
        string line;
        for (int i = 0; i < 3; i++)
        {
            getline(file, line);
            istringstream lineStream(line);
            config[i] = stoi(line);
        }
    }

    Board board(config[0], config[1], config[2]);
    return board;
}

int main()
{
    TextureManager::SetImagesDirectory("images/");
    Board board = LoadBoardFromConfig();
    sf::RenderWindow window(sf::VideoMode(board.GetWidth(), board.GetHeight()), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                auto position = sf::Mouse::getPosition(window);
                board.OnClick(event.mouseButton);

                cout << "Row: " << position.y / 32 << " Column:" << position.x / 32 << endl;
                cout << "click: xPos: " << position.x << " yPos: " << position.y << endl; 
            }
        }

        window.clear(sf::Color::White);
        // let the board handle the rendering
        board.Render(window);
        window.display();
    }

    // Clear out any textures before the program ends
    TextureManager::Clear();
    return 0;
}
